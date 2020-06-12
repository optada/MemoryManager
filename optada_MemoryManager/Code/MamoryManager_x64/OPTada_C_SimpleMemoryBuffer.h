#pragma once

#include "OPTada_C_TemplateMemoryBuffer.h"


// ����� - ����������� ������ ������ 
// ���������� ������������ ��� ����������� �������� ������ ��� ��� ������ �� ������ ���������� 
// ��� �������� �������� ����� ������ � ����������� �������, �������� �� ��� ����������� ����� � ������ ������������
class OPTada_C_SimpleMemoryBuffer : public OPTada_C_TemplateMemoryBuffer
{
protected:

	OPTada_S_CyclicMemoryElemsBufferElement * FirstCell_Buffer = NULL;   // ��������� �� ������ ������
	OPTada_S_CyclicMemoryElemsBufferElement * FreeCells_Buffer = NULL;   // ������ ��������� �����
	OPTada_S_CyclicMemoryElemsBufferElement * LockedCells_Buffer = NULL; // ������ ��������������� �����
	OPTada_C_CyclicMemoryElemsBuffer * Elem_Buffer = NULL; // ������ � �������� - ���������� (��� ������ � ���������)

	size_t Cell_Size = 0; // ���������� ������� ����������� (��� ���������� ������������) (����)

	// ����� ���� ��������� ������, ���� ������� - ��������� ��� ������ � ��������
	void * TakeMemoryMethod(size_t New_size_);

public:

	// ����������� OPTada_Memory_C_SimpleMemoryBuffer
	// ������ ����� ��������� � ��, �������������� ������ ����� ������
	// [in] size_t Size_ // ������ ������������ ������� (����)
	// [in] size_t Elem_Buffer_Size_ // ������ ������� ��������� (�������������� ������) (����������� ���������)
	// [in] size_t Cell_Size_ // ������ ������ (����������� �������) ��� ���������� ������������
	OPTada_C_SimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_);

	//���������� OPTada_Memory_C_SimpleMemoryBuffer
	~OPTada_C_SimpleMemoryBuffer();


	// !!! �������� !!! ������ ���������� ��������� � �� �������� ����������(�)
	// ����� ������� ��� ������ � ������� ��� ���������� ������ � ��� ������
	// return = true - ������ | false - ������
	bool Clear_Buffer();


	// !!! �������� !!! �� ����������������, ������ ���������� ������.
	// ����� ���������� ����� ��������� ����� ������ � ������� ��� (�������� ��� �������������) �� ������
	// [in] size_t New_Length_ // ����������� ����� ����� ������ (� ������)
	// return = ���� ������ ������ ��������� �� ������ | NULL - ���� �� �������
	void * GetMemory(size_t New_Length_);

	// !!! �������� !!! ���������� �� ����������, ������ ���������� ��� "���������"
	// ����� ������� ��������� ������� ������ ��� "���������"
	// [in] void * Link_ // ��������� �� ������ ������� ����� ����������
	// return = true - ������ | false - ������
	bool ReturnMemory(void * Link_);


	// ����� �������� ���� ������� �� ������� ������ ��� ��������
	// return = true - ������ ������ | false - ������ ��� �������� �������
	bool Get_TestBuffer();

	// ����� ���������� ����������� ������� ������ � ������� (� ������)
	// return = ����������� ������� ������ � ���� �������
	size_t Get_LockedMemory();

	// ����� ���������� ����������� ������� ������ (+ �������������� �������) (� ������)
	// return = ����� - ����������� ������ ������� � ������� ���� ������� � ��� ������������
	size_t Get_AllModulesLockedMemory();

	// ����� ���������� ������ ������� (� ������)
	// return = ����� - ������ ������� (� ������) | 0 - ������ �� ������/������
	size_t Get_Size();
};