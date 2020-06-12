#include "OPTada_Memory_C_CyclicMemoryElemsBuffer.h"

OPTada_Memory_S_CyclicMemoryElemsBufferElement::OPTada_Memory_S_CyclicMemoryElemsBufferElement()
{
	Size = 0;
}


OPTada_Memory_C_CyclicMemoryElemsBuffer::OPTada_Memory_C_CyclicMemoryElemsBuffer(size_t Size_)
{
	Buffer = (OPTada_Memory_S_CyclicMemoryElemsBufferElement *)malloc(sizeof(OPTada_Memory_S_CyclicMemoryElemsBufferElement) * Size_);
	Length = Size_;
	Locked = 0;
	Index = 0;
	if (Buffer != NULL)
	{
		for (int i = 0; i < Length; i++) // ������ ������������� ��� ��������-���������
			Buffer[i] = OPTada_Memory_S_CyclicMemoryElemsBufferElement();
		Buffer_last = &Buffer[Length];
	}
	else
	{
		Buffer_last = NULL;
		Length = 0;
		Index = 0;
	}
}

OPTada_Memory_C_CyclicMemoryElemsBuffer::~OPTada_Memory_C_CyclicMemoryElemsBuffer()
{
	if (Buffer != NULL)
	{
		free(Buffer);
		Buffer = NULL;
	}
}

OPTada_Memory_S_CyclicMemoryElemsBufferElement * OPTada_Memory_C_CyclicMemoryElemsBuffer::Get_Element()
{
	if (Length > Locked) // ���� ��������� ������
	{
		do
		{
			if (Buffer[Index].Size == 0) // ����� ��������� ������
			{
				OPTada_Memory_S_CyclicMemoryElemsBufferElement * FElem = &Buffer[Index];
				FElem->Size = 1; // ���� ��������� ����� �� ����� ����� ��� ������ � �� �� ���������� ���������
				FElem->free = true;
				Locked++;

				return FElem; // ��� � ������
			}
			else // ������ ������, ����� ���������
			{
				Index++;
				if (Index >= Length) // ���� �� ����� �� ������ �������
					Index = 0;
			}
		} while (true);
	}
	else // ���� ��������� ������
	{
		return NULL; 
	}
}

bool OPTada_Memory_C_CyclicMemoryElemsBuffer::Return_Element(OPTada_Memory_S_CyclicMemoryElemsBufferElement * Elem_)
{
	if (Elem_ != NULL && (Elem_ >= Buffer && Elem_ <= Buffer_last)) // ���� ����� ������ � ��� ��������� � ������ �������
	{
		Elem_->Size = 0; // ���� 0 - ������ ��������
		Locked--;
		// !! � ������ �������� �����!
		return true;
	}
	else // NULL - ������ ��� ��� ������ �� �� ����� �������
	{
		return false;
	}
}

size_t OPTada_Memory_C_CyclicMemoryElemsBuffer::Get_LockedAllMemory()
{
	return (sizeof(OPTada_Memory_C_CyclicMemoryElemsBuffer) + sizeof(OPTada_Memory_S_CyclicMemoryElemsBufferElement) * Length);
}