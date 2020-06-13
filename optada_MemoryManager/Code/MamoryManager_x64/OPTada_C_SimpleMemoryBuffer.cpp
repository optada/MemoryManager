#include "OPTada_C_SimpleMemoryBuffer.h"

void * OPTada_C_SimpleMemoryBuffer::TakeMemoryMethod(size_t New_size_)
{
	OPTadaS_MemoryCellElement * elem = FreeCells_Buffer;
	if (New_size_ < 1)
		return NULL;

	// добавление коэфициента к размеру
	size_t test_cell_size = New_size_ % Cell_Size;
	if (test_cell_size)
	{
		New_size_ += Cell_Size - test_cell_size;
	}

	while (FreeCells_Buffer)
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
						FreeCells_Buffer = elem->next_link;
						elem->next_link->previous_link = NULL;
					}
					else
					{ // нету следующего элемента
						FreeCells_Buffer = NULL;
					}
				}

				// сохранение размера занятой памяти
				Locked += New_size_;
				elem->isfree = false;
				// перенос в заблокированые
				if (LockedCells_Buffer)
				{ // если не NULL
					LockedCells_Buffer->previous_link = elem;
					elem->previous_link = NULL;
					elem->next_link = LockedCells_Buffer;
					LockedCells_Buffer = elem;
				}
				else
				{ // если NULL - просто добавить 
					elem->next_link = NULL;
					elem->previous_link = NULL;
					LockedCells_Buffer = elem;
				}

				return elem->link;
			}
			else
			{ // если не ноль - обновить free ячейку и создать lock ячейку

				// добавили новую ячейку для дальнейшего деления
				OPTadaS_MemoryCellElement * new_elem = Elem_Buffer->Get_Element();
				if (new_elem == NULL)
					return NULL;

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
					FirstCell_Buffer = new_elem;
				}
				
				// отделяем кусочек и меняем ссылки в основном элементе
				elem->size = (elem->size - New_size_);
				new_elem->size = New_size_;
				new_elem->link = elem->link;
				elem->link = &elem->link[New_size_];

				// сохранение размера занятой памяти
				Locked += New_size_;
				new_elem->isfree = false;

				// перенос в заблокированые
				if (LockedCells_Buffer)
				{ // если не NULL
					LockedCells_Buffer->previous_link = new_elem;
					new_elem->previous_link = NULL;
					new_elem->next_link = LockedCells_Buffer;
					LockedCells_Buffer = new_elem;
				}
				else
				{ // если NULL - просто добавить 
					new_elem->next_link = NULL;
					new_elem->previous_link = NULL;
					LockedCells_Buffer = new_elem;
				}

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
				return NULL;
			}
		}
	}
	return NULL;
}


OPTada_C_SimpleMemoryBuffer::OPTada_C_SimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_)
{
	Buffer = (char *)malloc(Size_); // создание буффера
	Elem_Buffer = (OPTadaC_MemoryCells_StaticCyclicBuffer *)malloc(sizeof(OPTadaC_MemoryCells_StaticCyclicBuffer)); // создание дополнительного класса-буффера
	Elem_Buffer->OPTadaC_MemoryCells_StaticCyclicBuffer::OPTadaC_MemoryCells_StaticCyclicBuffer(Elem_Buffer_Size_);
	Buffer_Length = Size_;
	Locked = 0;
	LockedCells_Buffer = NULL;
	Cell_Size = Cell_Size_;

	// все установленно, настройка первой ячейки
	FreeCells_Buffer = Elem_Buffer->Get_Element();
	FirstCell_Buffer = FreeCells_Buffer;
	if (FreeCells_Buffer)
	{
		FreeCells_Buffer->link = Buffer;
		FreeCells_Buffer->next_el = NULL;
		FreeCells_Buffer->previous_el = NULL;
		FreeCells_Buffer->next_link = NULL;
		FreeCells_Buffer->previous_link = NULL;
		FreeCells_Buffer->size = Size_;
	}
}

OPTada_C_SimpleMemoryBuffer::~OPTada_C_SimpleMemoryBuffer()
{
	if (Elem_Buffer != NULL)
	{
		Elem_Buffer->~OPTadaC_MemoryCells_StaticCyclicBuffer();
		free(Elem_Buffer);
		Elem_Buffer = NULL;
	}
	if (Buffer != NULL)
		free(Buffer);
}

bool OPTada_C_SimpleMemoryBuffer::Clear_Buffer()
{
	// проходим по всем ячейкам и возвращаем их
	OPTadaS_MemoryCellElement * cellDell = FirstCell_Buffer->next_el; // передаем ВТОРОЙ элемент

	if (!Elem_Buffer) // если нету буффера
		return false;

	while (cellDell)
	{
		if (cellDell->next_el) // если существует след элемент
		{
			cellDell = cellDell->next_el;
			Elem_Buffer->Return_Element(cellDell->previous_el);
		}
		else // если ето последний элемент
		{
			Elem_Buffer->Return_Element(cellDell);
		}
	}

	// теперь настройка буфферов заново
	LockedCells_Buffer = NULL;
	FreeCells_Buffer = FirstCell_Buffer;

	// настройка стартовой ячеки заново
	if (!FreeCells_Buffer)
	{
		FreeCells_Buffer->isfree = true;
		FreeCells_Buffer->link = Buffer;
		FreeCells_Buffer->next_el = NULL;
		FreeCells_Buffer->previous_el = NULL;
		FreeCells_Buffer->next_link = NULL;
		FreeCells_Buffer->previous_link = NULL;
		FreeCells_Buffer->size = Buffer_Length;
	}

	return true;
}

void * OPTada_C_SimpleMemoryBuffer::GetMemory(size_t New_Length_)
{
	return TakeMemoryMethod(New_Length_);
}

bool OPTada_C_SimpleMemoryBuffer::ReturnMemory(void * Link_)
{
	if (Link_ != NULL && (Link_ >= Buffer && Link_ <= &Buffer[Buffer_Length]))
	{ // нашло ссылку в нашем диапазоне буффера
		OPTadaS_MemoryCellElement * elem = LockedCells_Buffer;
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
						LockedCells_Buffer = elem->next_link;
						elem->next_link->previous_link = NULL;
					}
					else
					{ // нету следующего элемента
						LockedCells_Buffer = NULL;
					}
				}

				elem->next_link = NULL;
				elem->previous_link = NULL;
				Locked -= elem->size; // освободили размер
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
						Elem_Buffer->Return_Element(dell_elem); // удаляем элемент
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
									FreeCells_Buffer = elem->next_link;
									elem->next_link->previous_link = NULL;
								}
								else
								{ // нету следующего элемента
									FreeCells_Buffer = NULL;
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
							FirstCell_Buffer = elem;

						elem->size += dell_elem->size; // изменили общий размер
						elem->link = dell_elem->link;
						
						Elem_Buffer->Return_Element(dell_elem); // удаляем элемент

					}
				}
				if (!dell_elem) // если слить нескем - запихиваем в free
				{ // перенос в free
					if (FreeCells_Buffer)
					{ // если не NULL
						FreeCells_Buffer->previous_link = elem;
						elem->previous_link = NULL;
						elem->next_link = FreeCells_Buffer;
						FreeCells_Buffer = elem;
					}
					else
					{ // если NULL - просто добавить 
						elem->next_link = NULL;
						elem->previous_link = NULL;
						FreeCells_Buffer = elem;
					}
				}

				return true;
			}
			else
			{ // не нашло - следующий
				elem = elem->next_link;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

bool OPTada_C_SimpleMemoryBuffer::Get_TestBuffer()
{
	if (Buffer != NULL && Buffer_Length > 0 && FirstCell_Buffer != NULL && FreeCells_Buffer != NULL && Elem_Buffer != NULL && Cell_Size > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t OPTada_C_SimpleMemoryBuffer::Get_LockedMemory()
{
	if (Buffer_Length > 0)
		return Locked;
	else
		return 0;
}

size_t OPTada_C_SimpleMemoryBuffer::Get_AllModulesLockedMemory()
{
	if (Buffer_Length > 0)
		return (Elem_Buffer->Get_AllCapturedMemory() + Buffer_Length);
	else
		return 0;
}

size_t OPTada_C_SimpleMemoryBuffer::Get_Size()
{
	return Buffer_Length;
}
