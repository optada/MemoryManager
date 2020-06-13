#include "OPTadaC_SimpleMemoryBuffer.h"

void * OPTadaC_SimpleMemoryBuffer::TakeMemoryMethod(size_t New_size_)
{
	OPTadaS_MemoryCell_Element * elem = freeCells_Buffer;
	if (New_size_ < 1)
		return NULL;

	// ���������� ����������� � �������
	size_t test_cell_size = New_size_ % cellOfDefragmentation_Size;
	if (test_cell_size)
	{
		New_size_ += cellOfDefragmentation_Size - test_cell_size;
	}

	while (freeCells_Buffer)
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
						freeCells_Buffer = elem->next_link;
						elem->next_link->previous_link = NULL;
					}
					else
					{ // ���� ���������� ��������
						freeCells_Buffer = NULL;
					}
				}

				// ���������� ������� ������� ������
				lockedMemory += New_size_;
				elem->isfree = false;
				// ������� � ��������������
				if (lockedCells_Buffer)
				{ // ���� �� NULL
					lockedCells_Buffer->previous_link = elem;
					elem->previous_link = NULL;
					elem->next_link = lockedCells_Buffer;
					lockedCells_Buffer = elem;
				}
				else
				{ // ���� NULL - ������ �������� 
					elem->next_link = NULL;
					elem->previous_link = NULL;
					lockedCells_Buffer = elem;
				}

				return elem->link;
			}
			else
			{ // ���� �� ���� - �������� free ������ � ������� lock ������

				// �������� ����� ������ ��� ����������� �������
				OPTadaS_MemoryCell_Element * new_elem = cellBuffer->Get_Element();
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
					firstCell_Buffer = new_elem;
				}
				
				// �������� ������� � ������ ������ � �������� ��������
				elem->size = (elem->size - New_size_);
				new_elem->size = New_size_;
				new_elem->link = elem->link;
				elem->link = &elem->link[New_size_];

				// ���������� ������� ������� ������
				lockedMemory += New_size_;
				new_elem->isfree = false;

				// ������� � ��������������
				if (lockedCells_Buffer)
				{ // ���� �� NULL
					lockedCells_Buffer->previous_link = new_elem;
					new_elem->previous_link = NULL;
					new_elem->next_link = lockedCells_Buffer;
					lockedCells_Buffer = new_elem;
				}
				else
				{ // ���� NULL - ������ �������� 
					new_elem->next_link = NULL;
					new_elem->previous_link = NULL;
					lockedCells_Buffer = new_elem;
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


OPTadaC_SimpleMemoryBuffer::OPTadaC_SimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_)
{
	buffer = (char *)malloc(Size_); // �������� �������
	cellBuffer = (OPTadaC_MemoryCells_StaticCyclicBuffer *)malloc(sizeof(OPTadaC_MemoryCells_StaticCyclicBuffer)); // �������� ��������������� ������-�������
	bool asdadas = false; //TODO ������ �����
	cellBuffer->OPTadaC_MemoryCells_StaticCyclicBuffer::OPTadaC_MemoryCells_StaticCyclicBuffer(Elem_Buffer_Size_, asdadas); //TODO �������� �������� �� ������
	buffer_Length = Size_;
	lockedMemory = 0;
	lockedCells_Buffer = NULL;
	cellOfDefragmentation_Size = Cell_Size_;

	// ��� ������������, ��������� ������ ������
	freeCells_Buffer = cellBuffer->Get_Element();
	firstCell_Buffer = freeCells_Buffer;
	if (freeCells_Buffer)
	{
		freeCells_Buffer->link = buffer;
		freeCells_Buffer->next_el = NULL;
		freeCells_Buffer->previous_el = NULL;
		freeCells_Buffer->next_link = NULL;
		freeCells_Buffer->previous_link = NULL;
		freeCells_Buffer->size = Size_;
	}
}

OPTadaC_SimpleMemoryBuffer::~OPTadaC_SimpleMemoryBuffer()
{
	if (cellBuffer != NULL)
	{
		cellBuffer->~OPTadaC_MemoryCells_StaticCyclicBuffer();
		free(cellBuffer);
		cellBuffer = NULL;
	}
	if (buffer != NULL)
		free(buffer);
}

bool OPTadaC_SimpleMemoryBuffer::Clear_Buffer()
{
	// �������� �� ���� ������� � ���������� ��
	OPTadaS_MemoryCell_Element * cellDell = firstCell_Buffer->next_el; // �������� ������ �������

	if (!cellBuffer) // ���� ���� �������
		return false;

	while (cellDell)
	{
		if (cellDell->next_el) // ���� ���������� ���� �������
		{
			cellDell = cellDell->next_el;
			cellBuffer->Return_Element(cellDell->previous_el);
		}
		else // ���� ��� ��������� �������
		{
			cellBuffer->Return_Element(cellDell);
		}
	}

	// ������ ��������� �������� ������
	lockedCells_Buffer = NULL;
	freeCells_Buffer = firstCell_Buffer;

	// ��������� ��������� ����� ������
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

	return true;
}

void * OPTadaC_SimpleMemoryBuffer::GetMemory(size_t New_Length_)
{
	return TakeMemoryMethod(New_Length_);
}

bool OPTadaC_SimpleMemoryBuffer::ReturnMemory(void * Link_)
{
	if (Link_ != NULL && (Link_ >= buffer && Link_ <= &buffer[buffer_Length]))
	{ // ����� ������ � ����� ��������� �������
		OPTadaS_MemoryCell_Element * elem = lockedCells_Buffer;
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
						lockedCells_Buffer = elem->next_link;
						elem->next_link->previous_link = NULL;
					}
					else
					{ // ���� ���������� ��������
						lockedCells_Buffer = NULL;
					}
				}

				elem->next_link = NULL;
				elem->previous_link = NULL;
				lockedMemory -= elem->size; // ���������� ������
				elem->isfree = true;

				OPTadaS_MemoryCell_Element * dell_elem = NULL;

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
						cellBuffer->Return_Element(dell_elem); // ������� �������
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
									freeCells_Buffer = elem->next_link;
									elem->next_link->previous_link = NULL;
								}
								else
								{ // ���� ���������� ��������
									freeCells_Buffer = NULL;
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
							firstCell_Buffer = elem;

						elem->size += dell_elem->size; // �������� ����� ������
						elem->link = dell_elem->link;
						
						cellBuffer->Return_Element(dell_elem); // ������� �������

					}
				}
				if (!dell_elem) // ���� ����� ������ - ���������� � free
				{ // ������� � free
					if (freeCells_Buffer)
					{ // ���� �� NULL
						freeCells_Buffer->previous_link = elem;
						elem->previous_link = NULL;
						elem->next_link = freeCells_Buffer;
						freeCells_Buffer = elem;
					}
					else
					{ // ���� NULL - ������ �������� 
						elem->next_link = NULL;
						elem->previous_link = NULL;
						freeCells_Buffer = elem;
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

bool OPTadaC_SimpleMemoryBuffer::TestBuffer()
{
	if (buffer != NULL && buffer_Length > 0 && firstCell_Buffer != NULL && freeCells_Buffer != NULL && cellBuffer != NULL && cellOfDefragmentation_Size > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t OPTadaC_SimpleMemoryBuffer::Get_LockedMemory()
{
	if (buffer_Length > 0)
		return lockedMemory;
	else
		return 0;
}

size_t OPTadaC_SimpleMemoryBuffer::Get_AllCapturedMemory()
{
	if (buffer_Length > 0)
		return (cellBuffer->Get_AllCapturedMemory() + buffer_Length);
	else
		return 0;
}

size_t OPTadaC_SimpleMemoryBuffer::Get_BufferOfMemorySize()
{
	return buffer_Length;
}
