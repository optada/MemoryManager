// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_MultithreadedSimpleMemoryBuffer.h"


OPTadaC_MultithreadedSimpleMemoryBuffer::OPTadaC_MultithreadedSimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_, bool& initDoneWithNoErrors_) 
	: OPTadaC_SimpleMemoryBuffer(Size_, Elem_Buffer_Size_, Cell_Size_, initDoneWithNoErrors_)
{
	InitializeCriticalSection(&threadSynchronization); // инициализация критической секции
}

OPTadaC_MultithreadedSimpleMemoryBuffer::~OPTadaC_MultithreadedSimpleMemoryBuffer()
{
	DeleteCriticalSection(&threadSynchronization); // удаление критической секции
}


bool OPTadaC_MultithreadedSimpleMemoryBuffer::Clear_Buffer()
{
	EnterCriticalSection(&threadSynchronization); // запрет на доступ (для синхронизации)
	bool returnResult = OPTadaC_SimpleMemoryBuffer::Clear_Buffer();
	LeaveCriticalSection(&threadSynchronization); // разрешили доступ следующему потоку
	return returnResult;
}

void* OPTadaC_MultithreadedSimpleMemoryBuffer::GetMemory(size_t New_Length_)
{
	EnterCriticalSection(&threadSynchronization); // запрет на доступ (для синхронизации)
	void* newLink = TakeMemoryMethod(New_Length_);
	LeaveCriticalSection(&threadSynchronization); // разрешили доступ следующему потоку
	return newLink;
}

bool OPTadaC_MultithreadedSimpleMemoryBuffer::ReturnMemory(void* link_)
{
	EnterCriticalSection(&threadSynchronization); // запрет на доступ (для синхронизации)
	bool returnResult = OPTadaC_SimpleMemoryBuffer::ReturnMemory(link_);
	LeaveCriticalSection(&threadSynchronization); // разрешили доступ следующему потоку
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
{ // нашло ссылку в нашем диапазоне буффера
OPTadaS_MemoryCell_Element * elem = lockedCells_Buffer;
while (elem)
{
if (elem->link == Link_)
{ // нашло ссылку

// вырезаем из занятого
if (elem->previous_link)
{ // есть элемент (ближе к началу)
if (elem->next_link)
{ // есть следующий элемент
elem->next_link->previous_link = elem->previous_link;
elem->previous_link->next_link = elem->next_link;
}
else
{ // нету следующего элемента
elem->previous_link->next_link = NULL;
}
}
else
{ // нету элемента (ближе к началу)
if (elem->next_link)
{ // есть следующий элемент
lockedCells_Buffer = elem->next_link;
elem->next_link->previous_link = NULL;
}
else
{ // нету следующего элемента
lockedCells_Buffer = NULL;
}
}

elem->next_link = NULL;
elem->previous_link = NULL;
lockedMemory -= elem->size; // освободили размер
elem->isfree = true;

OPTadaS_MemoryCell_Element * dell_elem = NULL;

// ищем соседей для слития (если не нашли - просто перенос в free) - kill sam sel f :(
if (elem->previous_el) // поиск элемента слева (предидущий)
{
if (elem->previous_el->isfree)
{ // предидущий элемент готов для слития
elem->previous_el->next_el = elem->next_el;
if (elem->next_el != NULL)
elem->next_el->previous_el = elem->previous_el;

elem->previous_el->size += elem->size; // изменили общий размер
dell_elem = elem;
elem = elem->previous_el;
cellBuffer->Return_Element(dell_elem); // удаляем элемент
}
}
if (elem->next_el)
{
if (elem->next_el->isfree)
{ // следующий элемент готов для слияния
// Вырезаем элемент из free буффера

if (dell_elem) // если мы уже сливали элемент тогда вырезаем
{
// вырезаем из свободного
if (elem->previous_link)
{ // есть элемент (ближе к началу)
if (elem->next_link)
{ // есть следующий элемент
elem->next_link->previous_link = elem->previous_link;
elem->previous_link->next_link = elem->next_link;
}
else
{ // нету следующего элемента
elem->previous_link->next_link = NULL;
}
}
else
{ // нету элемента (ближе к началу)
if (elem->next_link)
{ // есть следующий элемент
freeCells_Buffer = elem->next_link;
elem->next_link->previous_link = NULL;
}
else
{ // нету следующего элемента
freeCells_Buffer = NULL;
}
}
}

elem = elem->next_el; // взяли ссылку и теперь работаем с предидущим элементом

// предидущий элемент готов для слития
dell_elem = elem->previous_el;
elem->previous_el = elem->previous_el->previous_el;
if (elem->previous_el != NULL)
elem->previous_el->next_el = elem;
else
firstCell_Buffer = elem;

elem->size += dell_elem->size; // изменили общий размер
elem->link = dell_elem->link;

cellBuffer->Return_Element(dell_elem); // удаляем элемент

}
}
if (!dell_elem) // если слить нескем - запихиваем в free
{ // перенос в free
if (freeCells_Buffer)
{ // если не NULL
freeCells_Buffer->previous_link = elem;
elem->previous_link = NULL;
elem->next_link = freeCells_Buffer;
freeCells_Buffer = elem;
}
else
{ // если NULL - просто добавить
elem->next_link = NULL;
elem->previous_link = NULL;
freeCells_Buffer = elem;
}
}

LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

return true;
}
else
{ // не нашло - следующий
elem = elem->next_link;
}
}

LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

return false;
}
else
{

LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

return false;
}
*/

// clear buffer inside
/*
EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

											  // проходим по всем ячейкам и возвращаем их
OPTadaS_MemoryCell_Element* cellDell = firstCell_Buffer->next_el; // передаем ВТОРОЙ элемент

if (!cellBuffer) // если нету буффера
{
	LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
	return false;
}

while (cellDell)
{
	if (cellDell->next_el) // если существует след элемент
	{
		cellDell = cellDell->next_el;
		cellBuffer->Return_Element(cellDell->previous_el);
	}
	else // если ето последний элемент
	{
		cellBuffer->Return_Element(cellDell);
	}
}

// теперь настройка бкфферов заново
lockedCells_Buffer = NULL;
freeCells_Buffer = firstCell_Buffer;

// настройка стартовой ячеки заново
if (!freeCells_Buffer) //TODO проверить логику
{
	freeCells_Buffer->isfree = true;
	freeCells_Buffer->link = buffer;
	freeCells_Buffer->next_el = NULL;
	freeCells_Buffer->previous_el = NULL;
	freeCells_Buffer->next_link = NULL;
	freeCells_Buffer->previous_link = NULL;
	freeCells_Buffer->size = buffer_Length;
}

LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку
return true;
*/