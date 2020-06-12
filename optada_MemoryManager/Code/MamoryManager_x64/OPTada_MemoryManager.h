#pragma once

#include "OPTada_Memory_C_TemplateMemoryBuffer.h"
#include "OPTada_Memory_C_SimpleMemoryBuffer.h"
#include "OPTada_Memory_C_MultithreadedSimpleMemoryBuffer.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// ��������!!! �������� �� ������ ����, �� ���� ������ �������� ��� ������� � �������������!!! //
// ���������� ��������� ������ ����� ��������												   //
//		������� ������																		   //
//		��������� ����������� ������ ������													   //
// ���� ��� ������� ������ �� �������� NULL - ������ ����� ������ ��� �������� �������!!!!	   //
// ��������� �������� ��� �������� ������� �� ������                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////


// ��������� ��� ������� � ������� ������ +
struct OPTada_Memory_S_MemoryManedger
{
	int nomer = -1; // ��������� �� ������������ ������ � ��������� ������
	int type = -1; // ���� -1 - �� ���������������, ���� 1 - < �� ��� ������ (�����? � - ��, ��� ����������� ��������)
};

// ��������� ��� ��������� ������ +
struct OPTada_Memory_S_MemoryManagerElement_Memory
{
public:
	OPTada_Memory_C_TemplateMemoryBuffer * Buffer = NULL; // ������ �� ����� ����� (������� - ������)
	bool Element_locked = false; // false - ������ �������� | true - ������������� (������)
	int BufferType = 0; // ��� �������
};

sss // �������� �������� ��������������� ��� �������� ����� �������

// �������� ������
// !! ������� ������������� !!
class OPTada_MemoryManager
{
private:

	OPTada_Memory_S_MemoryManagerElement_Memory* Elem_buffer = NULL; // ������ ����� - ���������
	int Size_buff = 0; // ����������� ����� � ������� (���� 0 - ������ �� ���������������)

public:

	OPTada_Memory_S_MemoryManedger New_Buffer; // ��������� �� ������� ������ (��� ������ �������������)

	// �������������� �������� ������ (���� �� ��� ��������������� - ��������� ������)
	// [in] int Size_buff_ // ����������� �������� ������� ����� ������� (+1 ������� new)
	// return = true - ������� | false - ������
	bool Init_Mamager(int Size_buff_);

	// ����������� ��� ���������� ������ � ��������� (������� ��� ������� � ����������� ������)
	// return = true - ������� | false - ������
	bool Free_Manager();


	// ����� ������� ����� ������ �� ��������� ������ ������ (����� 0) � ���������� Buffer_MemoryManedger *
	// [in] int type_ // ��� ������������ �������
	//		1 - �����������
	//		2 - ����������� � ���������� ���������������
	// [in] size_t Size_ // ����� ������ � ����������� �������
	// [in] size_t Elem_Buffer_Size_ // ����������� �����-�������� � ����������� �������
	// return = ��� ����� ������ ��������� �� ��������� ��� ��������� � ���������� ������ | NULL - ��� ������
	OPTada_Memory_S_MemoryManedger * CreateNewBuffer(int type_, size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_);


	// ����� �������� ������ �������������� ������� (������� ����� ��� �������������� ����������)
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������
	// [in] size_t Size_ // ������������� ����� ������
	// return = ��������� �� ������ � ������ ������ | NULL - ��� ������
	void * GetMemory(OPTada_Memory_S_MemoryManedger * Link_Buffer_, size_t Size_);


	// !!! �������� !!! ���������� �� ����������, ������ ���������� ��� "���������"
	// ����� ������� ��������� ������� ������ ��� "���������"
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������
	// [in] void * Link_ // ��������� �� ������ ������� ����� ����������
	// return = true - ������ | false - ������
	bool ReturnMemory(OPTada_Memory_S_MemoryManedger * Link_Buffer_, void * Link_);


	// !!! �������� !!! ������ ���������� ��������� � �� �������� ����������(�)
	// ����� ������� ��� ������ � ������� ��� ���������� ������ � ��� ������
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������
	// return = true - ������ | false - ������
	bool Clear_Buffer(OPTada_Memory_S_MemoryManedger * Link_Buffer_);


	// ����� ���������� ����������� ������� ������ � ������� (� ������)
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������, ���� NULL - �� ���� ��������
	// return = ����������� ������� ������ � ���� �������
	size_t Get_LockedMemory(OPTada_Memory_S_MemoryManedger * Link_Buffer_);

	// ����� ���������� ����������� ������� ������ ������ (+ �������������� ������� � ������ ����� ������) (� ������)
	// return = ����� - ����������� ������ ������� � ������� ���� ������� � ��� ������������
	size_t Get_AllModulesLockedMemory();

	// ����� ���������� ������ ������� (� ������)
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������
	// return = ����� - ������ ������� (� ������) | 0 - ������ �� ������/������
	size_t Get_Size(OPTada_Memory_S_MemoryManedger * Link_Buffer_);

	 
	//// ����������� OPTada_GSC_MemoryManager
	//OPTada_GSC_MemoryManager();
	//// ���������� OPTada_GSC_MemoryManager
	//~OPTada_GSC_MemoryManager();
};


//void *operator new(size_t size)
//{
//	return NULL;
//}

//void *operator new(size_t size, OPTada_Memory_S_MemoryManedger * Link_Buffer_)
//{
//	return NULL;
//}

//// !!! �������� !!! ���������� �� ����������, ������ ���������� ��� "���������"
//// ����� ������� ��������� ������� ������ ��� "���������" (��� ������ �� ������)
//// ����� ����������� ����� ���������
//// [in] void * Link_ // ��������� �� ������ ������� ����� ����������
//// return = true - ������ | false - ������ 
//bool ReturnMemoryMass(void * Link_)
//{
//	size_t * del_link = (size_t *)Link_;
//	return ReturnMemory((--del_link));
//}



//// !!! �������� !!! �� ����������������, ������ ���������� ������.
//// ����� ���������� ����� ��������� ����� ������ � ������� ��� (�������� ��� �������������) ��� �������
//// [in] size_t New_Length_ // ����������� ����� ����� ������ (� ������)
//// [in] size_t Col_mass_ // ����������� ��������� � ������ ������� ������ (��� ������ ������������)
//// return = ���� ������ ������ ��������� �� ������ | NULL - ���� �� �������
//void * GetMemoryMass(size_t New_Length_, size_t Col_mass_)
//{
//	size_t * link;
//	if (Col_mass_ > 0)
//	{ // ��� ������
//		if ((link = (size_t *)TakeMemoryMethod(sizeof(size_t) + New_Length_))) // �������� ��� 8 ���� ��� �������� ������� �������
//		{
//			*link = Col_mass_;
//			return (++link);
//		}
//		else
//		{
//			return NULL;
//		}
//	}
//	return NULL;
//}



