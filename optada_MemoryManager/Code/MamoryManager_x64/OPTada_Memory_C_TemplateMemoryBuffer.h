#pragma once

#include "OPTada_Memory_C_CyclicMemoryElemsBuffer.h"

// ����� - ������ ��� ��������� �������� +
class OPTada_Memory_C_TemplateMemoryBuffer
{
protected:

	char * Buffer = nullptr; // ��������� �� ������ ������
	size_t Buffer_Length = 0; // ����� ������ ������ ��������� � ���� ������� (� ������)
	size_t Locked = 0; // ����������� ������ ������� � ���� ������� (� ������)

public:

	virtual bool Clear_Buffer() = 0; // virtual ����� ������� ��� ������ � ������� ��� ���������� ������ � ��� ������ (! ������ ���������� ��������� � �� �������� �����������)

	virtual void * GetMemory(size_t New_Length_) = 0; // virtual ����� ���������� ����� ��������� ����� ������ � ������� ��� (�������� ��� �������������)
	virtual bool ReturnMemory(void * Link_) = 0; // virtual ����� ������� ��������� ������� ������ ��� "���������"

	virtual bool Get_TestBuffer() = 0; // virtual ����� �������� ���� ������� �� ������� ������ ��� ��������
	virtual size_t Get_LockedMemory() = 0; // virtual ����� ���������� ����������� ������� ������ � ������� (� ������)
	virtual size_t Get_AllModulesLockedMemory() = 0; // virtual ����� ���������� ����������� ������� ������ ������ (+ �������������� �������) (� ������)
	virtual size_t Get_Size() = 0; // virtual ����� ���������� ������ ������� (� ������)
};