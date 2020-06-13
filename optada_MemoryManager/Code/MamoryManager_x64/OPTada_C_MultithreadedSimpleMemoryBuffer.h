#pragma once

#include "OPTada_C_SimpleMemoryBuffer.h"


// Класс - стандартный буффер памяти с защитой данных для многопоточности
// Желательно использовать для одноразовой загрузки данных или для данных на полную перезапись 
// При запроссе пытается найти ячейку с достаточным объемом, отделяет от нее необходимый кусок и выдает пользователю
class OPTada_C_MultithreadedSimpleMemoryBuffer : public OPTadaC_SimpleMemoryBuffer
{
protected:

	CRITICAL_SECTION ThreadSynchronization; // критическая секция для синхронизации потоков

	// Метод ищет свободную память, если найдена - выполняет всю работу и выделяет
	void * TakeMemoryMethod(size_t New_size_)
	{

		EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

		OPTadaS_MemoryCellElement * elem = freeCells_Buffer;
		if (New_size_ < 1)
		{
			LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
			return NULL;
		}

		// добавление коэфициента к размеру
		size_t test_cell_size = New_size_ % cellOfDefragmentation_Size;
		if (test_cell_size)
		{
			New_size_ += cellOfDefragmentation_Size - test_cell_size;
		}

		while (freeCells_Buffer)
		{
			if (elem->size >= New_size_)
			{ // нашли нужный размер, начинаем делить...
				if (elem->size == New_size_)
				{ // если одинаково - просто перенеcти в lock

				  // вывод и связь компонентов в Free буфере (вырезаем элемент)
					if (elem->previous_link)
					{ // есть элемент (ближе к началу)
						if (elem->next_link)
						{ // есть следующий элемент
							elem->next_link->previous_link = elem->previous_link;
							elem->previous_link->next_link = elem->next_link;
						}
						else
						{ // нету следующего элемента
							elem->previous_link->next_link = NULL;
						}
					}
					else
					{ // нету элемента (ближе к началу)
						if (elem->next_link)
						{ // есть следующий элемент
							freeCells_Buffer = elem->next_link;
							elem->next_link->previous_link = NULL;
						}
						else
						{ // нету следующего элемента
							freeCells_Buffer = NULL;
						}
					}

					// сохранение размера занятой памяти
					lockedMemory += New_size_;
					elem->isfree = false;
					// перенос в заблокированые
					if (lockedCells_Buffer)
					{ // если не NULL
						lockedCells_Buffer->previous_link = elem;
						elem->previous_link = NULL;
						elem->next_link = lockedCells_Buffer;
						lockedCells_Buffer = elem;
					}
					else
					{ // если NULL - просто добавить 
						elem->next_link = NULL;
						elem->previous_link = NULL;
						lockedCells_Buffer = elem;
					}

					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
					return elem->link;
				}
				else
				{ // если не ноль - обновить free ячейку и создать lock ячейку

				  // добавили новую ячейку для дальнейшего деления
					OPTadaS_MemoryCellElement * new_elem = cellBuffer->Get_Element();
					if (new_elem == NULL)
					{
						LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
						return NULL;
					}

					if (elem->previous_el)
					{ // есть предидущая ячейка
						new_elem->previous_el = elem->previous_el;
						new_elem->next_el = elem;
						new_elem->previous_el->next_el = new_elem;
						elem->previous_el = new_elem;
					}
					else
					{ // нету
						new_elem->next_el = elem;
						elem->previous_el = new_elem;
						new_elem->previous_el = NULL;
						firstCell_Buffer = new_elem;
					}

					// отделяем кусочек и меняем ссылки в основном элементе
					elem->size = (elem->size - New_size_);
					new_elem->size = New_size_;
					new_elem->link = elem->link;
					elem->link = &elem->link[New_size_];

					// сохранение размера занятой памяти
					lockedMemory += New_size_;
					new_elem->isfree = false;

					// перенос в заблокированые
					if (lockedCells_Buffer)
					{ // если не NULL
						lockedCells_Buffer->previous_link = new_elem;
						new_elem->previous_link = NULL;
						new_elem->next_link = lockedCells_Buffer;
						lockedCells_Buffer = new_elem;
					}
					else
					{ // если NULL - просто добавить 
						new_elem->next_link = NULL;
						new_elem->previous_link = NULL;
						lockedCells_Buffer = new_elem;
					}

					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
					return new_elem->link;
				}
			}
			else
			{ // ищем дальше
				if (elem->next_link)
				{
					elem = elem->next_link;
				}
				else
				{
					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
					return NULL;
				}
			}
		}
		LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
		return NULL;
	}

public:

	// Конструктор OPTada_Memory_C_MultithreadedSimpleMemoryBuffer
	// Память будет запрошена у ОС, дополнительный буффер будет создан
	// [in] size_t Size_ // Размер создаваемого буффера (байт)
	// [in] size_t Elem_Buffer_Size_ // Размер буффера элементов (дополнительный буффер) (колличество элементов)
	// [in] size_t Cell_Size_ // Размер ячейки (коэфициента деления) для уменьшения фрагментации
	OPTada_C_MultithreadedSimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_) : OPTadaC_SimpleMemoryBuffer(Size_, Elem_Buffer_Size_, Cell_Size_)
	{
		InitializeCriticalSection(&ThreadSynchronization); // инициализация критической секции
	}

	// Деструктор OPTada_Memory_C_MultithreadedSimpleMemoryBuffer
	~OPTada_C_MultithreadedSimpleMemoryBuffer()
	{
		DeleteCriticalSection(&ThreadSynchronization); // удаление критической секции
	}


	// !!! ВНИМАНИЕ !!! память помечается свободной и не вызывает деструктор(ы)
	// Метод очищает всю память в буффере для дальнейшей работы с ней заново
	// return = true - удачно | false - ошибка
	bool Clear_Buffer()
	{
		EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

		// проходим по всем ячейкам и возвращаем их
		OPTadaS_MemoryCellElement * cellDell = firstCell_Buffer->next_el; // передаем ВТОРОЙ элемент

		if (!cellBuffer) // если нету буффера
		{
			LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
			return false;
		}

		while (cellDell)
		{
			if (cellDell->next_el) // если существует след элемент
			{
				cellDell = cellDell->next_el;
				cellBuffer->Return_Element(cellDell->previous_el);
			}
			else // если ето последний элемент
			{
				cellBuffer->Return_Element(cellDell);
			}
		}

		// теперь настройка бкфферов заново
		lockedCells_Buffer = NULL;
		freeCells_Buffer = firstCell_Buffer;

		// настройка стартовой ячеки заново
		if (!freeCells_Buffer)
		{
			freeCells_Buffer->isfree = true;
			freeCells_Buffer->link = buffer;
			freeCells_Buffer->next_el = NULL;
			freeCells_Buffer->previous_el = NULL;
			freeCells_Buffer->next_link = NULL;
			freeCells_Buffer->previous_link = NULL;
			freeCells_Buffer->size = buffer_Length;
		}

		LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
		return true;
	}


	// !!! ВНИМАНИЕ !!! не инициализируется, только выделяется память.
	// Метод попытается найти ближайший кусок памяти и вернуть его (выделить для использования) не массив
	// [in] size_t New_Length_ // необходимая длина куска памяти (в байтах)
	// return = если удачно вернет указатель на память | NULL - если не удалось
	void * GetMemory(size_t New_Length_)
	{
		return TakeMemoryMethod(New_Length_);
	}

	// !!! ВНИМАНИЕ !!! деструктор не вызывается, память помечается как "свободная"
	// Метод пометит указанный участок памяти как "свободная"
	// [in] void * Link_ // Указатель на память которую нужно освободить
	// return = true - удачно | false - ошибка
	bool ReturnMemory(void * Link_)
	{
		EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

		if (Link_ != NULL && (Link_ >= buffer && Link_ <= &buffer[buffer_Length]))
		{ // нашло ссылку в нашем диапазоне буффера
			OPTadaS_MemoryCellElement * elem = lockedCells_Buffer;
			while (elem)
			{
				if (elem->link == Link_)
				{ // нашло ссылку

				  // вырезаем из занятого
					if (elem->previous_link)
					{ // есть элемент (ближе к началу)
						if (elem->next_link)
						{ // есть следующий элемент
							elem->next_link->previous_link = elem->previous_link;
							elem->previous_link->next_link = elem->next_link;
						}
						else
						{ // нету следующего элемента
							elem->previous_link->next_link = NULL;
						}
					}
					else
					{ // нету элемента (ближе к началу)
						if (elem->next_link)
						{ // есть следующий элемент
							lockedCells_Buffer = elem->next_link;
							elem->next_link->previous_link = NULL;
						}
						else
						{ // нету следующего элемента
							lockedCells_Buffer = NULL;
						}
					}

					elem->next_link = NULL;
					elem->previous_link = NULL;
					lockedMemory -= elem->size; // освободили размер
					elem->isfree = true;

					OPTadaS_MemoryCellElement * dell_elem = NULL;

					// ищем соседей для слития (если не нашли - просто перенос в free) - kill sam sel f :(
					if (elem->previous_el) // поиск элемента слева (предидущий)
					{
						if (elem->previous_el->isfree)
						{ // предидущий элемент готов для слития
							elem->previous_el->next_el = elem->next_el;
							if (elem->next_el != NULL)
								elem->next_el->previous_el = elem->previous_el;

							elem->previous_el->size += elem->size; // изменили общий размер
							dell_elem = elem;
							elem = elem->previous_el;
							cellBuffer->Return_Element(dell_elem); // удаляем элемент
						}
					}
					if (elem->next_el)
					{
						if (elem->next_el->isfree)
						{ // следующий элемент готов для слияния
						  // Вырезаем элемент из free буффера						

							if (dell_elem) // если мы уже сливали элемент тогда вырезаем
							{
								// вырезаем из свободного
								if (elem->previous_link)
								{ // есть элемент (ближе к началу)
									if (elem->next_link)
									{ // есть следующий элемент
										elem->next_link->previous_link = elem->previous_link;
										elem->previous_link->next_link = elem->next_link;
									}
									else
									{ // нету следующего элемента
										elem->previous_link->next_link = NULL;
									}
								}
								else
								{ // нету элемента (ближе к началу)
									if (elem->next_link)
									{ // есть следующий элемент
										freeCells_Buffer = elem->next_link;
										elem->next_link->previous_link = NULL;
									}
									else
									{ // нету следующего элемента
										freeCells_Buffer = NULL;
									}
								}
							}

							elem = elem->next_el; // взяли ссылку и теперь работаем с предидущим элементом

												  // предидущий элемент готов для слития
							dell_elem = elem->previous_el;
							elem->previous_el = elem->previous_el->previous_el;
							if (elem->previous_el != NULL)
								elem->previous_el->next_el = elem;
							else
								firstCell_Buffer = elem;

							elem->size += dell_elem->size; // изменили общий размер
							elem->link = dell_elem->link;

							cellBuffer->Return_Element(dell_elem); // удаляем элемент

						}
					}
					if (!dell_elem) // если слить нескем - запихиваем в free
					{ // перенос в free
						if (freeCells_Buffer)
						{ // если не NULL
							freeCells_Buffer->previous_link = elem;
							elem->previous_link = NULL;
							elem->next_link = freeCells_Buffer;
							freeCells_Buffer = elem;
						}
						else
						{ // если NULL - просто добавить 
							elem->next_link = NULL;
							elem->previous_link = NULL;
							freeCells_Buffer = elem;
						}
					}

					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

					return true;
				}
				else
				{ // не нашло - следующий
					elem = elem->next_link;
				}
			}

			LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

			return false;
		}
		else
		{

			LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

			return false;
		}
	}
};
