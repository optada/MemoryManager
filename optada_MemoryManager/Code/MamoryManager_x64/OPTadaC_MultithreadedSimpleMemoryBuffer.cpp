// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_MultithreadedSimpleMemoryBuffer.h"


OPTadaC_MultithreadedSimpleMemoryBuffer::OPTadaC_MultithreadedSimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_, bool& initDoneWithNoErrors_) 
	: OPTadaC_SimpleMemoryBuffer(Size_, Elem_Buffer_Size_, Cell_Size_, initDoneWithNoErrors_)
{
	InitializeCriticalSection(&threadSynchronization); // ������������� ����������� ������
}

OPTadaC_MultithreadedSimpleMemoryBuffer::~OPTadaC_MultithreadedSimpleMemoryBuffer()
{
	DeleteCriticalSection(&threadSynchronization); // �������� ����������� ������
}


bool OPTadaC_MultithreadedSimpleMemoryBuffer::Clear_Buffer()
{
	EnterCriticalSection(&threadSynchronization); // ������ �� ������ (��� �������������)
	bool returnResult = OPTadaC_SimpleMemoryBuffer::Clear_Buffer();
	LeaveCriticalSection(&threadSynchronization); // ��������� ������ ���������� ������
	return returnResult;
}

void* OPTadaC_MultithreadedSimpleMemoryBuffer::GetMemory(size_t New_Length_)
{
	EnterCriticalSection(&threadSynchronization); // ������ �� ������ (��� �������������)
	void* newLink = TakeMemoryMethod(New_Length_);
	LeaveCriticalSection(&threadSynchronization); // ��������� ������ ���������� ������
	return newLink;
}

bool OPTadaC_MultithreadedSimpleMemoryBuffer::ReturnMemory(void* link_)
{
	EnterCriticalSection(&threadSynchronization); // ������ �� ������ (��� �������������)
	bool returnResult = OPTadaC_SimpleMemoryBuffer::ReturnMemory(link_);
	LeaveCriticalSection(&threadSynchronization); // ��������� ������ ���������� ������
	return returnResult;
}

size_t OPTadaC_MultithreadedSimpleMemoryBuffer::Get_AllModulesLockedMemory()
{
	if (buffer_Length > 0)
		return (cellBuffer->Get_AllCapturedMemory() + buffer_Length + sizeof(OPTadaC_MultithreadedSimpleMemoryBuffer));
	else
		return 0;
}

// return memory inside
/*
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

LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������

return true;
}
else
{ // �� ����� - ���������
elem = elem->next_link;
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
*/

// clear buffer inside
/*
EnterCriticalSection(&ThreadSynchronization); // ������ �� ������ (��� �������������)

											  // �������� �� ���� ������� � ���������� ��
OPTadaS_MemoryCell_Element* cellDell = firstCell_Buffer->next_el; // �������� ������ �������

if (!cellBuffer) // ���� ���� �������
{
	LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
	return false;
}

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
if (!freeCells_Buffer) //TODO ��������� ������
{
	freeCells_Buffer->isfree = true;
	freeCells_Buffer->link = buffer;
	freeCells_Buffer->next_el = NULL;
	freeCells_Buffer->previous_el = NULL;
	freeCells_Buffer->next_link = NULL;
	freeCells_Buffer->previous_link = NULL;
	freeCells_Buffer->size = buffer_Length;
}

LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������
return true;
*/