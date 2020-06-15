// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaC_SimpleMemoryBuffer.h"


// Class - standard memory buffer with security data for multithreading
// Upon request, it tries to find a cell with sufficient volume, separates the necessary piece from it and gives it to the user
// Has automatic defragmentation tools for free areas
class OPTadaC_MultithreadedSimpleMemoryBuffer : public OPTadaC_SimpleMemoryBuffer
{
protected:

	CRITICAL_SECTION threadSynchronization; // critical section for thread synchronization

public:

	// Память будет запрошена у ОС, дополнительный буффер будет создан
	// [in]   size_t size_                // Размер создаваемого буффера (байт)
	// [in]   size_t elem_Buffer_Size_    // Размер буффера элементов (дополнительный буффер) (колличество элементов)
	// [in]   size_t cell_Size_           // Размер ячейки (коэфициента деления) для уменьшения фрагментации
	// [out]  bool&  initDoneWithNoErrors // to verify the creation of the buffer
	// [CALL] : OPTadaC_SimpleMemoryBuffer(size_, elem_Buffer_Size_, cell_Size_, initDoneWithNoErrors_);
	OPTadaC_MultithreadedSimpleMemoryBuffer(size_t size_, size_t elem_Buffer_Size_, size_t cell_Size_, bool& initDoneWithNoErrors_);

	// Деструктор OPTada_Memory_C_MultithreadedSimpleMemoryBuffer
	~OPTadaC_MultithreadedSimpleMemoryBuffer();


	// !!! ВНИМАНИЕ !!! память помечается свободной и не вызывает деструктор(ы)
	// Метод очищает всю память в буффере для дальнейшей работы с ней заново
	// return = true - удачно | false - ошибка
	bool Clear_Buffer();

	// !!! ВНИМАНИЕ !!! не инициализируется, только выделяется память.
	// Метод попытается найти ближайший кусок памяти и вернуть его (выделить для использования) не массив
	// [in] size_t new_Length_ // необходимая длина куска памяти (в байтах)
	// return = если удачно вернет указатель на память | NULL - если не удалось
	void* GetMemory(size_t new_Length_);

	// !!! ВНИМАНИЕ !!! деструктор не вызывается, память помечается как "свободная"
	// Метод пометит указанный участок памяти как "свободная"
	// [in] void* link_ // Указатель на память которую нужно освободить
	// return = true - удачно | false - ошибка
	bool ReturnMemory(void* link_);

	// The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	// return = number - the amount of memory used by the system and its components (in bytes)
	size_t Get_AllModulesLockedMemory();
};



/*
// The method searches for free memory, if found, does all the work and allocates
//void * TakeMemoryMethod(size_t New_size_)
//{

//	EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

//	OPTadaS_MemoryCell_Element * elem = freeCells_Buffer;
//	if (New_size_ < 1)
//	{
//		LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
//		return NULL;
//	}

//	// добавление коэфициента к размеру
//	size_t test_cell_size = New_size_ % cellOfDefragmentation_Size;
//	if (test_cell_size)
//	{
//		New_size_ += cellOfDefragmentation_Size - test_cell_size;
//	}

//	while (freeCells_Buffer)
//	{
//		if (elem->size >= New_size_)
//		{ // нашли нужный размер, начинаем делить...
//			if (elem->size == New_size_)
//			{ // если одинаково - просто перенеcти в lock

//			  // вывод и связь компонентов в Free буфере (вырезаем элемент)
//				if (elem->previous_link)
//				{ // есть элемент (ближе к началу)
//					if (elem->next_link)
//					{ // есть следующий элемент
//						elem->next_link->previous_link = elem->previous_link;
//						elem->previous_link->next_link = elem->next_link;
//					}
//					else
//					{ // нету следующего элемента
//						elem->previous_link->next_link = NULL;
//					}
//				}
//				else
//				{ // нету элемента (ближе к началу)
//					if (elem->next_link)
//					{ // есть следующий элемент
//						freeCells_Buffer = elem->next_link;
//						elem->next_link->previous_link = NULL;
//					}
//					else
//					{ // нету следующего элемента
//						freeCells_Buffer = NULL;
//					}
//				}

//				// сохранение размера занятой памяти
//				lockedMemory += New_size_;
//				elem->isfree = false;
//				// перенос в заблокированые
//				if (lockedCells_Buffer)
//				{ // если не NULL
//					lockedCells_Buffer->previous_link = elem;
//					elem->previous_link = NULL;
//					elem->next_link = lockedCells_Buffer;
//					lockedCells_Buffer = elem;
//				}
//				else
//				{ // если NULL - просто добавить
//					elem->next_link = NULL;
//					elem->previous_link = NULL;
//					lockedCells_Buffer = elem;
//				}

//				LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
//				return elem->link;
//			}
//			else
//			{ // если не ноль - обновить free ячейку и создать lock ячейку

//			  // добавили новую ячейку для дальнейшего деления
//				OPTadaS_MemoryCell_Element * new_elem = cellBuffer->Get_Element();
//				if (new_elem == NULL)
//				{
//					LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
//					return NULL;
//				}

//				if (elem->previous_el)
//				{ // есть предидущая ячейка
//					new_elem->previous_el = elem->previous_el;
//					new_elem->next_el = elem;
//					new_elem->previous_el->next_el = new_elem;
//					elem->previous_el = new_elem;
//				}
//				else
//				{ // нету
//					new_elem->next_el = elem;
//					elem->previous_el = new_elem;
//					new_elem->previous_el = NULL;
//					firstCell_Buffer = new_elem;
//				}

//				// отделяем кусочек и меняем ссылки в основном элементе
//				elem->size = (elem->size - New_size_);
//				new_elem->size = New_size_;
//				new_elem->link = elem->link;
//				elem->link = &elem->link[New_size_];

//				// сохранение размера занятой памяти
//				lockedMemory += New_size_;
//				new_elem->isfree = false;

//				// перенос в заблокированые
//				if (lockedCells_Buffer)
//				{ // если не NULL
//					lockedCells_Buffer->previous_link = new_elem;
//					new_elem->previous_link = NULL;
//					new_elem->next_link = lockedCells_Buffer;
//					lockedCells_Buffer = new_elem;
//				}
//				else
//				{ // если NULL - просто добавить
//					new_elem->next_link = NULL;
//					new_elem->previous_link = NULL;
//					lockedCells_Buffer = new_elem;
//				}

//				LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
//				return new_elem->link;
//			}
//		}
//		else
//		{ // ищем дальше
//			if (elem->next_link)
//			{
//				elem = elem->next_link;
//			}
//			else
//			{
//				LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
//				return NULL;
//			}
//		}
//	}
//	LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
//	return NULL;
//}
*/