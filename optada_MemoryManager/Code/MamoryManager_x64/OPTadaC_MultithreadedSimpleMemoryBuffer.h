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

	// ������ ����� ��������� � ��, �������������� ������ ����� ������
	// [in]   size_t size_                // ������ ������������ ������� (����)
	// [in]   size_t elem_Buffer_Size_    // ������ ������� ��������� (�������������� ������) (����������� ���������)
	// [in]   size_t cell_Size_           // ������ ������ (����������� �������) ��� ���������� ������������
	// [out]  bool&  initDoneWithNoErrors // to verify the creation of the buffer
	// [CALL] : OPTadaC_SimpleMemoryBuffer(size_, elem_Buffer_Size_, cell_Size_, initDoneWithNoErrors_);
	OPTadaC_MultithreadedSimpleMemoryBuffer(size_t size_, size_t elem_Buffer_Size_, size_t cell_Size_, bool& initDoneWithNoErrors_);

	// ���������� OPTada_Memory_C_MultithreadedSimpleMemoryBuffer
	~OPTadaC_MultithreadedSimpleMemoryBuffer();


	// !!! �������� !!! ������ ���������� ��������� � �� �������� ����������(�)
	// ����� ������� ��� ������ � ������� ��� ���������� ������ � ��� ������
	// return = true - ������ | false - ������
	bool Clear_Buffer();

	// !!! �������� !!! �� ����������������, ������ ���������� ������.
	// ����� ���������� ����� ��������� ����� ������ � ������� ��� (�������� ��� �������������) �� ������
	// [in] size_t new_Length_ // ����������� ����� ����� ������ (� ������)
	// return = ���� ������ ������ ��������� �� ������ | NULL - ���� �� �������
	void* GetMemory(size_t new_Length_);

	// !!! �������� !!! ���������� �� ����������, ������ ���������� ��� "���������"
	// ����� ������� ��������� ������� ������ ��� "���������"
	// [in] void* link_ // ��������� �� ������ ������� ����� ����������
	// return = true - ������ | false - ������
	bool ReturnMemory(void* link_);

	// The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	// return = number - the amount of memory used by the system and its components (in bytes)
	size_t Get_AllModulesLockedMemory();
};



/*
// The method searches for free memory, if found, does all the work and allocates
//void * TakeMemoryMethod(size_t New_size_)
//{

//	EnterCriticalSection(&ThreadSynchronization); // ������ �� ������ (��� �������������)

//	OPTadaS_MemoryCell_Element * elem = freeCells_Buffer;
//	if (New_size_ < 1)
//	{
//		LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
//		return NULL;
//	}

//	// ���������� ����������� � �������
//	size_t test_cell_size = New_size_ % cellOfDefragmentation_Size;
//	if (test_cell_size)
//	{
//		New_size_ += cellOfDefragmentation_Size - test_cell_size;
//	}

//	while (freeCells_Buffer)
//	{
//		if (elem->size >= New_size_)
//		{ // ����� ������ ������, �������� ������...
//			if (elem->size == New_size_)
//			{ // ���� ��������� - ������ ������c�� � lock

//			  // ����� � ����� ����������� � Free ������ (�������� �������)
//				if (elem->previous_link)
//				{ // ���� ������� (����� � ������)
//					if (elem->next_link)
//					{ // ���� ��������� �������
//						elem->next_link->previous_link = elem->previous_link;
//						elem->previous_link->next_link = elem->next_link;
//					}
//					else
//					{ // ���� ���������� ��������
//						elem->previous_link->next_link = NULL;
//					}
//				}
//				else
//				{ // ���� �������� (����� � ������)
//					if (elem->next_link)
//					{ // ���� ��������� �������
//						freeCells_Buffer = elem->next_link;
//						elem->next_link->previous_link = NULL;
//					}
//					else
//					{ // ���� ���������� ��������
//						freeCells_Buffer = NULL;
//					}
//				}

//				// ���������� ������� ������� ������
//				lockedMemory += New_size_;
//				elem->isfree = false;
//				// ������� � ��������������
//				if (lockedCells_Buffer)
//				{ // ���� �� NULL
//					lockedCells_Buffer->previous_link = elem;
//					elem->previous_link = NULL;
//					elem->next_link = lockedCells_Buffer;
//					lockedCells_Buffer = elem;
//				}
//				else
//				{ // ���� NULL - ������ ��������
//					elem->next_link = NULL;
//					elem->previous_link = NULL;
//					lockedCells_Buffer = elem;
//				}

//				LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
//				return elem->link;
//			}
//			else
//			{ // ���� �� ���� - �������� free ������ � ������� lock ������

//			  // �������� ����� ������ ��� ����������� �������
//				OPTadaS_MemoryCell_Element * new_elem = cellBuffer->Get_Element();
//				if (new_elem == NULL)
//				{
//					LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
//					return NULL;
//				}

//				if (elem->previous_el)
//				{ // ���� ���������� ������
//					new_elem->previous_el = elem->previous_el;
//					new_elem->next_el = elem;
//					new_elem->previous_el->next_el = new_elem;
//					elem->previous_el = new_elem;
//				}
//				else
//				{ // ����
//					new_elem->next_el = elem;
//					elem->previous_el = new_elem;
//					new_elem->previous_el = NULL;
//					firstCell_Buffer = new_elem;
//				}

//				// �������� ������� � ������ ������ � �������� ��������
//				elem->size = (elem->size - New_size_);
//				new_elem->size = New_size_;
//				new_elem->link = elem->link;
//				elem->link = &elem->link[New_size_];

//				// ���������� ������� ������� ������
//				lockedMemory += New_size_;
//				new_elem->isfree = false;

//				// ������� � ��������������
//				if (lockedCells_Buffer)
//				{ // ���� �� NULL
//					lockedCells_Buffer->previous_link = new_elem;
//					new_elem->previous_link = NULL;
//					new_elem->next_link = lockedCells_Buffer;
//					lockedCells_Buffer = new_elem;
//				}
//				else
//				{ // ���� NULL - ������ ��������
//					new_elem->next_link = NULL;
//					new_elem->previous_link = NULL;
//					lockedCells_Buffer = new_elem;
//				}

//				LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
//				return new_elem->link;
//			}
//		}
//		else
//		{ // ���� ������
//			if (elem->next_link)
//			{
//				elem = elem->next_link;
//			}
//			else
//			{
//				LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
//				return NULL;
//			}
//		}
//	}
//	LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
//	return NULL;
//}
*/