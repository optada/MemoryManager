// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTada_MemoryManager_Settings.h"

#include "SupportBuffers\OPTadaS_TemplateMemoryBuffer_Element.h"

#include "SupportBuffers\OPTadaC_TemplateOfMemoryBuffer.h"
#include "SupportBuffers\OPTadaC_SimpleMemoryBuffer.h"
#include "SupportBuffers\OPTadaC_MultithreadedSimpleMemoryBuffer.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
// ��������!!! �������� �� ������ ����, �� ���� ������ �������� ��� ������� � �������������!!! //
// ���������� ��������� ������ ����� ��������												   //
//		������� ������																		   //
//		��������� ����������� ������ ������													   //
// ���� ��� ������� ������ �� �������� nullptr - ������ ����� ������ ��� �������� �������!!!!  //
// ��������� �������� ��� �������� ������� �� ������                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////


// ��������� ��� ������� � ������� ������ +
struct OPTadaS_MemoryManedger
{
	int nomer = -1; // ��������� �� ������������ ������ � ��������� ������
	int type = -1;  // ���� -1 - �� ���������������, ���� 1 - < �� ��� ������ (�����? � - ��, ��� ����������� ��������)
};


//TODO // �������� �������� ��������������� ��� �������� ����� �������






// �������� ������
// !! ������� ������������� !!
class OPTada_MemoryManager
{
private:

	OPTadaS_TemplateMemoryBuffer_Element* buffer_OfMemoryBuffers = nullptr; // ������ ����� - ���������
	int size_buffer_OfMemoryBuffers                              = 0;       // ����������� ����� � ������� (���� 0 - ������ �� ���������������)

	CRITICAL_SECTION threadSynchronization; // critical section for thread synchronization

public:

	// �������������� �������� ������ (���� �� ��� ��������������� - ��������� ������)
	// [in] int Size_buff_ // ����������� �������� ������� ����� ������� (+1 default new)
	// return = true - ������� | false - ������
	bool Init_Mamager(int Size_buff_);

	// ����������� ��� ���������� ������ � ��������� (������� ��� ������� � ����������� ������)
	// return = true - ������� | false - ������
	bool Free_Manager();


	// ����� ������� ����� ������ �� ��������� ������ ������ (����� 0) � ���������� Buffer_MemoryManedger *
	// [in] int type_ // ��� ������������ �������
	// [in] size_t Size_ // ����� ������ � ����������� �������
	// [in] size_t Elem_Buffer_Size_ // ����������� �����-�������� � ����������� �������
	// return = ��� ����� ������ ��������� �� ��������� ��� ��������� � ���������� ������ | nullptr - ��� ������
	OPTadaS_MemoryManedger* CreateNewMemoryBuffer(int type_, size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_);

	// ����� ������� � ��������� ������� ���������� ������ ������
	// [in] OPTadaS_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������
	// return = true - ������� | false - ������
	bool DeleteMemoryBuffer(OPTadaS_MemoryManedger* link_Buffer_);


	// !- memory is marked free and does not call destructors -!
	// The method clears all the memory in the buffer for further work with it again
	// [in] OPTadaS_MemoryManedger* link_Buffer_ // Structure to indicate the required buffer
	// return = true - successful | false - error
	bool Clear_Buffer(OPTadaS_MemoryManedger* link_Buffer_);

	// ����� �������� ������ �������������� ������� (������� ����� ��� �������������� ����������)
	// [in] OPTadaS_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������
	// [in] size_t Size_ // ������������� ����� ������
	// return = ��������� �� ������ � ������ ������ | nullptr - ��� ������
	void* GetMemory(OPTadaS_MemoryManedger* Link_Buffer_, size_t Size_);

	// !!! �������� !!! ���������� �� ����������, ������ ���������� ��� "���������"
	// ����� ������� ��������� ������� ������ ��� "���������"
	// [in] OPTadaS_MemoryManedger * Link_Buffer_ // ��������� ��� �������� �� ����������� ������
	// [in] void * Link_ // ��������� �� ������ ������� ����� ����������
	// return = true - ������ | false - ������
	bool ReturnMemory(OPTadaS_MemoryManedger* Link_Buffer_, void * Link_);

	// The method returns the count of locked memory in the buffer (in bytes)
	// [in] OPTadaS_MemoryManedger* link_Buffer_ // Structure to indicate the required buffer, if nullptr - from all buffers
	// return = count of memory used (locked) in this buffer
	size_t Get_LockedMemory(OPTadaS_MemoryManedger* link_Buffer_);

	// The method returns the size of the memory buffer (in bytes)
	// [in] OPTadaS_MemoryManedger* link_Buffer_ // Structure to indicate the required buffer, if nullptr - from all buffers
	// return = number - buffer size(in bytes) | 0 - buffer not created / error
	size_t Get_BufferMemorySize(OPTadaS_MemoryManedger* link_Buffer_);

	// The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	// return = number - the amount of memory used by the system and its components (in bytes)
	size_t Get_AllModulesLockedMemory();
};


//void *operator new(size_t size)
//{
//	return nullptr;
//}

//void *operator new(size_t size, OPTada_Memory_S_MemoryManedger * Link_Buffer_)
//{
//	return nullptr;
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
//// return = ���� ������ ������ ��������� �� ������ | nullptr - ���� �� �������
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
//			return nullptr;
//		}
//	}
//	return nullptr;
//}



