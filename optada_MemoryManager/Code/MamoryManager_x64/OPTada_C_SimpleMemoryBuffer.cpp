#include "OPTada_C_SimpleMemoryBuffer.h"

void * OPTada_C_SimpleMemoryBuffer::TakeMemoryMethod(size_t New_size_)
{
	OPTadaS_MemoryCellElement * elem = FreeCells_Buffer;
	if (New_size_ < 1)
		return NULL;

	// ���������� ����������� � �������
	size_t test_cell_size = New_size_ % Cell_Size;
	if (test_cell_size)
	{
		New_size_ += Cell_Size - test_cell_size;
	}

	while (FreeCells_Buffer)
	{
		if (elem->size >= New_size_)
		{ // ����� ������ ������, �������� ������...
			if (elem->size == New_size_)
			{ // ���� ��������� - ������ ������c�� � lock

				// ����� � ����� ����������� � Free ������ (�������� �������)
				if (elem->previous_link)
				{ // ���� ������� (����� � ������)
					if (elem->next_link)
					{ // ���� ��������� �������
						elem->next_link->previous_link = elem->previous_link;
						elem->previous_link->next_link = elem->next_link;
					}
					else
					{ // ���� ���������� ��������
						elem->previous_link->next_link = NULL;
					}
				}
				else
				{ // ���� �������� (����� � ������)
					if (elem->next_link)
					{ // ���� ��������� �������
						FreeCells_Buffer = elem->next_link;
						elem->next_link->previous_link = NULL;
					}
					else
					{ // ���� ���������� ��������
						FreeCells_Buffer = NULL;
					}
				}

				// ���������� ������� ������� ������
				Locked += New_size_;
				elem->isfree = false;
				// ������� � ��������������
				if (LockedCells_Buffer)
				{ // ���� �� NULL
					LockedCells_Buffer->previous_link = elem;
					elem->previous_link = NULL;
					elem->next_link = LockedCells_Buffer;
					LockedCells_Buffer = elem;
				}
				else
				{ // ���� NULL - ������ �������� 
					elem->next_link = NULL;
					elem->previous_link = NULL;
					LockedCells_Buffer = elem;
				}

				return elem->link;
			}
			else
			{ // ���� �� ���� - �������� free ������ � ������� lock ������

				// �������� ����� ������ ��� ����������� �������
				OPTadaS_MemoryCellElement * new_elem = Elem_Buffer->Get_Element();
				if (new_elem == NULL)
					return NULL;

				if (elem->previous_el)
				{ // ���� ���������� ������
					new_elem->previous_el = elem->previous_el;
					new_elem->next_el = elem;
					new_elem->previous_el->next_el = new_elem;
					elem->previous_el = new_elem;
				}
				else
				{ // ����
					new_elem->next_el = elem;
					elem->previous_el = new_elem;
					new_elem->previous_el = NULL;
					FirstCell_Buffer = new_elem;
				}
				
				// �������� ������� � ������ ������ � �������� ��������
				elem->size = (elem->size - New_size_);
				new_elem->size = New_size_;
				new_elem->link = elem->link;
				elem->link = &elem->link[New_size_];

				// ���������� ������� ������� ������
				Locked += New_size_;
				new_elem->isfree = false;

				// ������� � ��������������
				if (LockedCells_Buffer)
				{ // ���� �� NULL
					LockedCells_Buffer->previous_link = new_elem;
					new_elem->previous_link = NULL;
					new_elem->next_link = LockedCells_Buffer;
					LockedCells_Buffer = new_elem;
				}
				else
				{ // ���� NULL - ������ �������� 
					new_elem->next_link = NULL;
					new_elem->previous_link = NULL;
					LockedCells_Buffer = new_elem;
				}

				return new_elem->link;
			}			
		}
		else
		{ // ���� ������
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
	Buffer = (char *)malloc(Size_); // �������� �������
	Elem_Buffer = (OPTadaC_MemoryCells_StaticCyclicBuffer *)malloc(sizeof(OPTadaC_MemoryCells_StaticCyclicBuffer)); // �������� ��������������� ������-�������
	Elem_Buffer->OPTadaC_MemoryCells_StaticCyclicBuffer::OPTadaC_MemoryCells_StaticCyclicBuffer(Elem_Buffer_Size_);
	Buffer_Length = Size_;
	Locked = 0;
	LockedCells_Buffer = NULL;
	Cell_Size = Cell_Size_;

	// ��� ������������, ��������� ������ ������
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
	// �������� �� ���� ������� � ���������� ��
	OPTadaS_MemoryCellElement * cellDell = FirstCell_Buffer->next_el; // �������� ������ �������

	if (!Elem_Buffer) // ���� ���� �������
		return false;

	while (cellDell)
	{
		if (cellDell->next_el) // ���� ���������� ���� �������
		{
			cellDell = cellDell->next_el;
			Elem_Buffer->Return_Element(cellDell->previous_el);
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
	{ // ����� ������ � ����� ��������� �������
		OPTadaS_MemoryCellElement * elem = LockedCells_Buffer;
		while (elem)
		{
			if (elem->link == Link_)
			{ // ����� ������

				// �������� �� ��������
				if (elem->previous_link)
				{ // ���� ������� (����� � ������)
					if (elem->next_link)
					{ // ���� ��������� �������
						elem->next_link->previous_link = elem->previous_link;
						elem->previous_link->next_link = elem->next_link;
					}
					else
					{ // ���� ���������� ��������
						elem->previous_link->next_link = NULL;
					}
				}
				else
				{ // ���� �������� (����� � ������)
					if (elem->next_link)
					{ // ���� ��������� �������
						LockedCells_Buffer = elem->next_link;
						elem->next_link->previous_link = NULL;
					}
					else
					{ // ���� ���������� ��������
						LockedCells_Buffer = NULL;
					}
				}

				elem->next_link = NULL;
				elem->previous_link = NULL;
				Locked -= elem->size; // ���������� ������
				elem->isfree = true;

				OPTadaS_MemoryCellElement * dell_elem = NULL;

				// ���� ������� ��� ������ (���� �� ����� - ������ ������� � free) - kill sam sel f :(
				if (elem->previous_el) // ����� �������� ����� (����������)
				{ 
					if (elem->previous_el->isfree)
					{ // ���������� ������� ����� ��� ������
						elem->previous_el->next_el = elem->next_el;
						if (elem->next_el != NULL)
							elem->next_el->previous_el = elem->previous_el;

						elem->previous_el->size += elem->size; // �������� ����� ������
						dell_elem = elem;
						elem = elem->previous_el;
						Elem_Buffer->Return_Element(dell_elem); // ������� �������
					}
				}
				if (elem->next_el)
				{
					if (elem->next_el->isfree)
					{ // ��������� ������� ����� ��� �������
						// �������� ������� �� free �������						

						if (dell_elem) // ���� �� ��� ������� ������� ����� ��������
						{
							// �������� �� ����������
							if (elem->previous_link)
							{ // ���� ������� (����� � ������)
								if (elem->next_link)
								{ // ���� ��������� �������
									elem->next_link->previous_link = elem->previous_link;
									elem->previous_link->next_link = elem->next_link;
								}
								else
								{ // ���� ���������� ��������
									elem->previous_link->next_link = NULL;
								}
							}
							else
							{ // ���� �������� (����� � ������)
								if (elem->next_link)
								{ // ���� ��������� �������
									FreeCells_Buffer = elem->next_link;
									elem->next_link->previous_link = NULL;
								}
								else
								{ // ���� ���������� ��������
									FreeCells_Buffer = NULL;
								}
							}
						}

						elem = elem->next_el; // ����� ������ � ������ �������� � ���������� ���������

						// ���������� ������� ����� ��� ������
						dell_elem = elem->previous_el;
						elem->previous_el = elem->previous_el->previous_el;
						if (elem->previous_el != NULL)
							elem->previous_el->next_el = elem;
						else
							FirstCell_Buffer = elem;

						elem->size += dell_elem->size; // �������� ����� ������
						elem->link = dell_elem->link;
						
						Elem_Buffer->Return_Element(dell_elem); // ������� �������

					}
				}
				if (!dell_elem) // ���� ����� ������ - ���������� � free
				{ // ������� � free
					if (FreeCells_Buffer)
					{ // ���� �� NULL
						FreeCells_Buffer->previous_link = elem;
						elem->previous_link = NULL;
						elem->next_link = FreeCells_Buffer;
						FreeCells_Buffer = elem;
					}
					else
					{ // ���� NULL - ������ �������� 
						elem->next_link = NULL;
						elem->previous_link = NULL;
						FreeCells_Buffer = elem;
					}
				}

				return true;
			}
			else
			{ // �� ����� - ���������
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
