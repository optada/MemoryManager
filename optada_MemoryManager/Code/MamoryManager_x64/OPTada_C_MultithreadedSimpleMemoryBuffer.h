#pragma once

#include "OPTada_C_SimpleMemoryBuffer.h"


// ����� - ����������� ������ ������ � ������� ������ ��� ���������������
// ���������� ������������ ��� ����������� �������� ������ ��� ��� ������ �� ������ ���������� 
// ��� �������� �������� ����� ������ � ����������� �������, �������� �� ��� ����������� ����� � ������ ������������
class OPTada_C_MultithreadedSimpleMemoryBuffer : public OPTada_C_SimpleMemoryBuffer
{
protected:

	CRITICAL_SECTION ThreadSynchronization; // ����������� ������ ��� ������������� �������

	// ����� ���� ��������� ������, ���� ������� - ��������� ��� ������ � ��������
	void * TakeMemoryMethod(size_t New_size_)
	{

		EnterCriticalSection(&ThreadSynchronization); // ������ �� ������ (��� �������������)

		OPTada_S_CyclicMemoryElemsBufferElement * elem = FreeCells_Buffer;
		if (New_size_ < 1)
		{
			LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
			return NULL;
		}

		// ���������� ����������� � �������
		size_t test_cell_size = New_size_ % Cell_Size;
		if (test_cell_size)
		{
			New_size_ += Cell_Size - test_cell_size;
		}

		while (FreeCells_Buffer)
		{
			if (elem->Size >= New_size_)
			{ // ����� ������ ������, �������� ������...
				if (elem->Size == New_size_)
				{ // ���� ��������� - ������ ������c�� � lock

				  // ����� � ����� ����������� � Free ������ (�������� �������)
					if (elem->Previous_link)
					{ // ���� ������� (����� � ������)
						if (elem->Next_link)
						{ // ���� ��������� �������
							elem->Next_link->Previous_link = elem->Previous_link;
							elem->Previous_link->Next_link = elem->Next_link;
						}
						else
						{ // ���� ���������� ��������
							elem->Previous_link->Next_link = NULL;
						}
					}
					else
					{ // ���� �������� (����� � ������)
						if (elem->Next_link)
						{ // ���� ��������� �������
							FreeCells_Buffer = elem->Next_link;
							elem->Next_link->Previous_link = NULL;
						}
						else
						{ // ���� ���������� ��������
							FreeCells_Buffer = NULL;
						}
					}

					// ���������� ������� ������� ������
					Locked += New_size_;
					elem->free = false;
					// ������� � ��������������
					if (LockedCells_Buffer)
					{ // ���� �� NULL
						LockedCells_Buffer->Previous_link = elem;
						elem->Previous_link = NULL;
						elem->Next_link = LockedCells_Buffer;
						LockedCells_Buffer = elem;
					}
					else
					{ // ���� NULL - ������ �������� 
						elem->Next_link = NULL;
						elem->Previous_link = NULL;
						LockedCells_Buffer = elem;
					}

					LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
					return elem->Link;
				}
				else
				{ // ���� �� ���� - �������� free ������ � ������� lock ������

				  // �������� ����� ������ ��� ����������� �������
					OPTada_S_CyclicMemoryElemsBufferElement * new_elem = Elem_Buffer->Get_Element();
					if (new_elem == NULL)
					{
						LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
						return NULL;
					}

					if (elem->Previous_el)
					{ // ���� ���������� ������
						new_elem->Previous_el = elem->Previous_el;
						new_elem->Next_el = elem;
						new_elem->Previous_el->Next_el = new_elem;
						elem->Previous_el = new_elem;
					}
					else
					{ // ����
						new_elem->Next_el = elem;
						elem->Previous_el = new_elem;
						new_elem->Previous_el = NULL;
						FirstCell_Buffer = new_elem;
					}

					// �������� ������� � ������ ������ � �������� ��������
					elem->Size = (elem->Size - New_size_);
					new_elem->Size = New_size_;
					new_elem->Link = elem->Link;
					elem->Link = &elem->Link[New_size_];

					// ���������� ������� ������� ������
					Locked += New_size_;
					new_elem->free = false;

					// ������� � ��������������
					if (LockedCells_Buffer)
					{ // ���� �� NULL
						LockedCells_Buffer->Previous_link = new_elem;
						new_elem->Previous_link = NULL;
						new_elem->Next_link = LockedCells_Buffer;
						LockedCells_Buffer = new_elem;
					}
					else
					{ // ���� NULL - ������ �������� 
						new_elem->Next_link = NULL;
						new_elem->Previous_link = NULL;
						LockedCells_Buffer = new_elem;
					}

					LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
					return new_elem->Link;
				}
			}
			else
			{ // ���� ������
				if (elem->Next_link)
				{
					elem = elem->Next_link;
				}
				else
				{
					LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
					return NULL;
				}
			}
		}
		LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
		return NULL;
	}

public:

	// ����������� OPTada_Memory_C_MultithreadedSimpleMemoryBuffer
	// ������ ����� ��������� � ��, �������������� ������ ����� ������
	// [in] size_t Size_ // ������ ������������ ������� (����)
	// [in] size_t Elem_Buffer_Size_ // ������ ������� ��������� (�������������� ������) (����������� ���������)
	// [in] size_t Cell_Size_ // ������ ������ (����������� �������) ��� ���������� ������������
	OPTada_C_MultithreadedSimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_) : OPTada_C_SimpleMemoryBuffer(Size_, Elem_Buffer_Size_, Cell_Size_)
	{
		InitializeCriticalSection(&ThreadSynchronization); // ������������� ����������� ������
	}

	// ���������� OPTada_Memory_C_MultithreadedSimpleMemoryBuffer
	~OPTada_C_MultithreadedSimpleMemoryBuffer()
	{
		DeleteCriticalSection(&ThreadSynchronization); // �������� ����������� ������
	}


	// !!! �������� !!! ������ ���������� ��������� � �� �������� ����������(�)
	// ����� ������� ��� ������ � ������� ��� ���������� ������ � ��� ������
	// return = true - ������ | false - ������
	bool Clear_Buffer()
	{
		EnterCriticalSection(&ThreadSynchronization); // ������ �� ������ (��� �������������)

		// �������� �� ���� ������� � ���������� ��
		OPTada_S_CyclicMemoryElemsBufferElement * cellDell = FirstCell_Buffer->Next_el; // �������� ������ �������

		if (!Elem_Buffer) // ���� ���� �������
		{
			LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
			return false;
		}

		while (cellDell)
		{
			if (cellDell->Next_el) // ���� ���������� ���� �������
			{
				cellDell = cellDell->Next_el;
				Elem_Buffer->Return_Element(cellDell->Previous_el);
			}
			else // ���� ��� ��������� �������
			{
				Elem_Buffer->Return_Element(cellDell);
			}
		}

		// ������ ��������� �������� ������
		LockedCells_Buffer = NULL;
		FreeCells_Buffer = FirstCell_Buffer;

		// ��������� ��������� ����� ������
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

		LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
		return true;
	}


	// !!! �������� !!! �� ����������������, ������ ���������� ������.
	// ����� ���������� ����� ��������� ����� ������ � ������� ��� (�������� ��� �������������) �� ������
	// [in] size_t New_Length_ // ����������� ����� ����� ������ (� ������)
	// return = ���� ������ ������ ��������� �� ������ | NULL - ���� �� �������
	void * GetMemory(size_t New_Length_)
	{
		return TakeMemoryMethod(New_Length_);
	}

	// !!! �������� !!! ���������� �� ����������, ������ ���������� ��� "���������"
	// ����� ������� ��������� ������� ������ ��� "���������"
	// [in] void * Link_ // ��������� �� ������ ������� ����� ����������
	// return = true - ������ | false - ������
	bool ReturnMemory(void * Link_)
	{
		EnterCriticalSection(&ThreadSynchronization); // ������ �� ������ (��� �������������)

		if (Link_ != NULL && (Link_ >= Buffer && Link_ <= &Buffer[Buffer_Length]))
		{ // ����� ������ � ����� ��������� �������
			OPTada_S_CyclicMemoryElemsBufferElement * elem = LockedCells_Buffer;
			while (elem)
			{
				if (elem->Link == Link_)
				{ // ����� ������

				  // �������� �� ��������
					if (elem->Previous_link)
					{ // ���� ������� (����� � ������)
						if (elem->Next_link)
						{ // ���� ��������� �������
							elem->Next_link->Previous_link = elem->Previous_link;
							elem->Previous_link->Next_link = elem->Next_link;
						}
						else
						{ // ���� ���������� ��������
							elem->Previous_link->Next_link = NULL;
						}
					}
					else
					{ // ���� �������� (����� � ������)
						if (elem->Next_link)
						{ // ���� ��������� �������
							LockedCells_Buffer = elem->Next_link;
							elem->Next_link->Previous_link = NULL;
						}
						else
						{ // ���� ���������� ��������
							LockedCells_Buffer = NULL;
						}
					}

					elem->Next_link = NULL;
					elem->Previous_link = NULL;
					Locked -= elem->Size; // ���������� ������
					elem->free = true;

					OPTada_S_CyclicMemoryElemsBufferElement * dell_elem = NULL;

					// ���� ������� ��� ������ (���� �� ����� - ������ ������� � free) - kill sam sel f :(
					if (elem->Previous_el) // ����� �������� ����� (����������)
					{
						if (elem->Previous_el->free)
						{ // ���������� ������� ����� ��� ������
							elem->Previous_el->Next_el = elem->Next_el;
							if (elem->Next_el != NULL)
								elem->Next_el->Previous_el = elem->Previous_el;

							elem->Previous_el->Size += elem->Size; // �������� ����� ������
							dell_elem = elem;
							elem = elem->Previous_el;
							Elem_Buffer->Return_Element(dell_elem); // ������� �������
						}
					}
					if (elem->Next_el)
					{
						if (elem->Next_el->free)
						{ // ��������� ������� ����� ��� �������
						  // �������� ������� �� free �������						

							if (dell_elem) // ���� �� ��� ������� ������� ����� ��������
							{
								// �������� �� ����������
								if (elem->Previous_link)
								{ // ���� ������� (����� � ������)
									if (elem->Next_link)
									{ // ���� ��������� �������
										elem->Next_link->Previous_link = elem->Previous_link;
										elem->Previous_link->Next_link = elem->Next_link;
									}
									else
									{ // ���� ���������� ��������
										elem->Previous_link->Next_link = NULL;
									}
								}
								else
								{ // ���� �������� (����� � ������)
									if (elem->Next_link)
									{ // ���� ��������� �������
										FreeCells_Buffer = elem->Next_link;
										elem->Next_link->Previous_link = NULL;
									}
									else
									{ // ���� ���������� ��������
										FreeCells_Buffer = NULL;
									}
								}
							}

							elem = elem->Next_el; // ����� ������ � ������ �������� � ���������� ���������

												  // ���������� ������� ����� ��� ������
							dell_elem = elem->Previous_el;
							elem->Previous_el = elem->Previous_el->Previous_el;
							if (elem->Previous_el != NULL)
								elem->Previous_el->Next_el = elem;
							else
								FirstCell_Buffer = elem;

							elem->Size += dell_elem->Size; // �������� ����� ������
							elem->Link = dell_elem->Link;

							Elem_Buffer->Return_Element(dell_elem); // ������� �������

						}
					}
					if (!dell_elem) // ���� ����� ������ - ���������� � free
					{ // ������� � free
						if (FreeCells_Buffer)
						{ // ���� �� NULL
							FreeCells_Buffer->Previous_link = elem;
							elem->Previous_link = NULL;
							elem->Next_link = FreeCells_Buffer;
							FreeCells_Buffer = elem;
						}
						else
						{ // ���� NULL - ������ �������� 
							elem->Next_link = NULL;
							elem->Previous_link = NULL;
							FreeCells_Buffer = elem;
						}
					}

					LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������

					return true;
				}
				else
				{ // �� ����� - ���������
					elem = elem->Next_link;
				}
			}

			LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������

			return false;
		}
		else
		{

			LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������

			return false;
		}
	}
};
