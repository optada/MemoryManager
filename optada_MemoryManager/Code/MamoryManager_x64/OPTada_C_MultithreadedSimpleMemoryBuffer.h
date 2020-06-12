#pragma once

#include "OPTada_C_SimpleMemoryBuffer.h"


// Класс - стандартный буффер памяти с защитой данных для многопоточности
// Желательно использовать для одноразовой загрузки данных или для данных на полную перезапись 
// При запроссе пытается найти ячейку с достаточным объемом, отделяет от нее необходимый кусок и выдает пользователю
class OPTada_C_MultithreadedSimpleMemoryBuffer : public OPTada_C_SimpleMemoryBuffer
{
protected:

	CRITICAL_SECTION ThreadSynchronization; // критическая секция для синхронизации потоков

	// Метод ищет свободную память, если найдена - выполняет всю работу и выделяет
	void * TakeMemoryMethod(size_t New_size_)
	{

		EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

		OPTada_S_CyclicMemoryElemsBufferElement * elem = FreeCells_Buffer;
		if (New_size_ < 1)
		{
			LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
			return NULL;
		}

		// добавление коэфициента к размеру
		size_t test_cell_size = New_size_ % Cell_Size;
		if (test_cell_size)
		{
			New_size_ += Cell_Size - test_cell_size;
		}

		while (FreeCells_Buffer)
		{
			if (elem->Size >= New_size_)
			{ // нашли нужный размер, начинаем делить...
				if (elem->Size == New_size_)
				{ // если одинаково - просто перенеcти в lock

				  // вывод и связь компонентов в Free буфере (вырезаем элемент)
					if (elem->Previous_link)
					{ // есть элемент (ближе к началу)
						if (elem->Next_link)
						{ // есть следующий элемент
							elem->Next_link->Previous_link = elem->Previous_link;
							elem->Previous_link->Next_link = elem->Next_link;
						}
						else
						{ // нету следующего элемента
							elem->Previous_link->Next_link = NULL;
						}
					}
					else
					{ // нету элемента (ближе к началу)
						if (elem->Next_link)
						{ // есть следующий элемент
							FreeCells_Buffer = elem->Next_link;
							elem->Next_link->Previous_link = NULL;
						}
						else
						{ // нету следующего элемента
							FreeCells_Buffer = NULL;
						}
					}

					// сохранение размера занятой памяти
					Locked += New_size_;
					elem->free = false;
					// перенос в заблокированые
					if (LockedCells_Buffer)
					{ // если не NULL
						LockedCells_Buffer->Previous_link = elem;
						elem->Previous_link = NULL;
						elem->Next_link = LockedCells_Buffer;
						LockedCells_Buffer = elem;
					}
					else
					{ // если NULL - просто добавить 
						elem->Next_link = NULL;
						elem->Previous_link = NULL;
						LockedCells_Buffer = elem;
					}

					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
					return elem->Link;
				}
				else
				{ // если не ноль - обновить free ячейку и создать lock ячейку

				  // добавили новую ячейку для дальнейшего деления
					OPTada_S_CyclicMemoryElemsBufferElement * new_elem = Elem_Buffer->Get_Element();
					if (new_elem == NULL)
					{
						LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
						return NULL;
					}

					if (elem->Previous_el)
					{ // есть предидущая ячейка
						new_elem->Previous_el = elem->Previous_el;
						new_elem->Next_el = elem;
						new_elem->Previous_el->Next_el = new_elem;
						elem->Previous_el = new_elem;
					}
					else
					{ // нету
						new_elem->Next_el = elem;
						elem->Previous_el = new_elem;
						new_elem->Previous_el = NULL;
						FirstCell_Buffer = new_elem;
					}

					// отделяем кусочек и меняем ссылки в основном элементе
					elem->Size = (elem->Size - New_size_);
					new_elem->Size = New_size_;
					new_elem->Link = elem->Link;
					elem->Link = &elem->Link[New_size_];

					// сохранение размера занятой памяти
					Locked += New_size_;
					new_elem->free = false;

					// перенос в заблокированые
					if (LockedCells_Buffer)
					{ // если не NULL
						LockedCells_Buffer->Previous_link = new_elem;
						new_elem->Previous_link = NULL;
						new_elem->Next_link = LockedCells_Buffer;
						LockedCells_Buffer = new_elem;
					}
					else
					{ // если NULL - просто добавить 
						new_elem->Next_link = NULL;
						new_elem->Previous_link = NULL;
						LockedCells_Buffer = new_elem;
					}

					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
					return new_elem->Link;
				}
			}
			else
			{ // ищем дальше
				if (elem->Next_link)
				{
					elem = elem->Next_link;
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
	OPTada_C_MultithreadedSimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_) : OPTada_C_SimpleMemoryBuffer(Size_, Elem_Buffer_Size_, Cell_Size_)
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
		OPTada_S_CyclicMemoryElemsBufferElement * cellDell = FirstCell_Buffer->Next_el; // передаем ВТОРОЙ элемент

		if (!Elem_Buffer) // если нету буффера
		{
			LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
			return false;
		}

		while (cellDell)
		{
			if (cellDell->Next_el) // если существует след элемент
			{
				cellDell = cellDell->Next_el;
				Elem_Buffer->Return_Element(cellDell->Previous_el);
			}
			else // если ето последний элемент
			{
				Elem_Buffer->Return_Element(cellDell);
			}
		}

		// теперь настройка бкфферов заново
		LockedCells_Buffer = NULL;
		FreeCells_Buffer = FirstCell_Buffer;

		// настройка стартовой ячеки заново
		if (!FreeCells_Buffer)
		{
			FreeCells_Buffer->free = true;
			FreeCells_Buffer->Link = Buffer;
			FreeCells_Buffer->Next_el = NULL;
			FreeCells_Buffer->Previous_el = NULL;
			FreeCells_Buffer->Next_link = NULL;
			FreeCells_Buffer->Previous_link = NULL;
			FreeCells_Buffer->Size = Buffer_Length;
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

		if (Link_ != NULL && (Link_ >= Buffer && Link_ <= &Buffer[Buffer_Length]))
		{ // нашло ссылку в нашем диапазоне буффера
			OPTada_S_CyclicMemoryElemsBufferElement * elem = LockedCells_Buffer;
			while (elem)
			{
				if (elem->Link == Link_)
				{ // нашло ссылку

				  // вырезаем из занятого
					if (elem->Previous_link)
					{ // есть элемент (ближе к началу)
						if (elem->Next_link)
						{ // есть следующий элемент
							elem->Next_link->Previous_link = elem->Previous_link;
							elem->Previous_link->Next_link = elem->Next_link;
						}
						else
						{ // нету следующего элемента
							elem->Previous_link->Next_link = NULL;
						}
					}
					else
					{ // нету элемента (ближе к началу)
						if (elem->Next_link)
						{ // есть следующий элемент
							LockedCells_Buffer = elem->Next_link;
							elem->Next_link->Previous_link = NULL;
						}
						else
						{ // нету следующего элемента
							LockedCells_Buffer = NULL;
						}
					}

					elem->Next_link = NULL;
					elem->Previous_link = NULL;
					Locked -= elem->Size; // освободили размер
					elem->free = true;

					OPTada_S_CyclicMemoryElemsBufferElement * dell_elem = NULL;

					// ищем соседей для слития (если не нашли - просто перенос в free) - kill sam sel f :(
					if (elem->Previous_el) // поиск элемента слева (предидущий)
					{
						if (elem->Previous_el->free)
						{ // предидущий элемент готов для слития
							elem->Previous_el->Next_el = elem->Next_el;
							if (elem->Next_el != NULL)
								elem->Next_el->Previous_el = elem->Previous_el;

							elem->Previous_el->Size += elem->Size; // изменили общий размер
							dell_elem = elem;
							elem = elem->Previous_el;
							Elem_Buffer->Return_Element(dell_elem); // удаляем элемент
						}
					}
					if (elem->Next_el)
					{
						if (elem->Next_el->free)
						{ // следующий элемент готов для слияния
						  // Вырезаем элемент из free буффера						

							if (dell_elem) // если мы уже сливали элемент тогда вырезаем
							{
								// вырезаем из свободного
								if (elem->Previous_link)
								{ // есть элемент (ближе к началу)
									if (elem->Next_link)
									{ // есть следующий элемент
										elem->Next_link->Previous_link = elem->Previous_link;
										elem->Previous_link->Next_link = elem->Next_link;
									}
									else
									{ // нету следующего элемента
										elem->Previous_link->Next_link = NULL;
									}
								}
								else
								{ // нету элемента (ближе к началу)
									if (elem->Next_link)
									{ // есть следующий элемент
										FreeCells_Buffer = elem->Next_link;
										elem->Next_link->Previous_link = NULL;
									}
									else
									{ // нету следующего элемента
										FreeCells_Buffer = NULL;
									}
								}
							}

							elem = elem->Next_el; // взяли ссылку и теперь работаем с предидущим элементом

												  // предидущий элемент готов для слития
							dell_elem = elem->Previous_el;
							elem->Previous_el = elem->Previous_el->Previous_el;
							if (elem->Previous_el != NULL)
								elem->Previous_el->Next_el = elem;
							else
								FirstCell_Buffer = elem;

							elem->Size += dell_elem->Size; // изменили общий размер
							elem->Link = dell_elem->Link;

							Elem_Buffer->Return_Element(dell_elem); // удаляем элемент

						}
					}
					if (!dell_elem) // если слить нескем - запихиваем в free
					{ // перенос в free
						if (FreeCells_Buffer)
						{ // если не NULL
							FreeCells_Buffer->Previous_link = elem;
							elem->Previous_link = NULL;
							elem->Next_link = FreeCells_Buffer;
							FreeCells_Buffer = elem;
						}
						else
						{ // если NULL - просто добавить 
							elem->Next_link = NULL;
							elem->Previous_link = NULL;
							FreeCells_Buffer = elem;
						}
					}

					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

					return true;
				}
				else
				{ // не нашло - следующий
					elem = elem->Next_link;
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
