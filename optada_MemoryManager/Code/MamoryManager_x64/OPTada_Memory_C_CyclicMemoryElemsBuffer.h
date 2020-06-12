#pragma once

#include "MemoryManager_Settings.h"

// ��������� - ������� ��� �������� ����a��� ������� (56 �)
struct OPTada_Memory_S_CyclicMemoryElemsBufferElement
{
public:
	size_t Size = 0;    // ����� ������ � ������ (���� 0 - ������ ��������)
	char * Link = NULL; // ��������� �� ������
	bool free = true;   // ���������� ������������ ��� ������ (��������?\���?)

	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Next_el = NULL;     // ��������� �� ��������� �������
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Previous_el = NULL; // ��������� �� ���������� ������� 

	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Next_link = NULL;     // ��������� �� ��������� ������� (�� ������ �������)
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Previous_link = NULL; // ��������� �� ���������� ������� (�� ������ �������)

	// ����������� OPTada_S_CiclicBufferElement
	OPTada_Memory_S_CyclicMemoryElemsBufferElement();
};



// ��������� ���� ����� �������� ��� �������� ������
// ����� - ����������� ������ �������� ������ OPTada_Memory_S_CyclicMemoryElemsBufferElement (��� �������� �������� ������� �������� ��������� ������)
class OPTada_Memory_C_CyclicMemoryElemsBuffer
{
private:

	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Buffer = NULL;      // ����� ���������
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Buffer_last = NULL; // ��������� �� ������ ���������� �������� � �������
	size_t Length = 0; // ������ ����������� �����
	size_t Locked = 0; // ����������� ���������� �����
	size_t Index  = 0; // ��������� �� ������� ������� � ������� (�� ��������� ���������, ����� 0)	

public:

	// ����������� OPTada_Memory_C_CyclicMemoryElemsBuffer
	// [in] size_t Size_ // ������ ������������ ������� - ����������� ����������� �����
	OPTada_Memory_C_CyclicMemoryElemsBuffer(size_t Size_);

	// ���������� OPTada_Memory_C_CyclicMemoryElemsBuffer
	~OPTada_Memory_C_CyclicMemoryElemsBuffer();


	// ������ �� ��������� �������
	// return = ������ �� ������� | NULL - ������ ������� ���������� (������)
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Get_Element(); 

	// ���������� ������� � ���� (����������� ���) - !! � ������ �������� ����� !!
	// [in] OPTada_Memory_S_CyclicMemoryElemsBufferElement * Elem_ // ��������� �� �������
	// return = true - ������� | false - ������
	bool Return_Element(OPTada_Memory_S_CyclicMemoryElemsBufferElement * Elem_);

	// ���������� ����������� ������ ������������ ���� ������� (+ ���������� ������)
	// return = ����������� ���������� ������ ���� ������� (������� ����������) (� ������)
	size_t Get_LockedAllMemory();
};