// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_MultithreadedSimpleMemoryBuffer.h"


OPTadaC_MultithreadedSimpleMemoryBuffer::OPTadaC_MultithreadedSimpleMemoryBuffer(size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_, bool& initDoneWithNoErrors_) 
	: OPTadaC_SimpleMemoryBuffer(memoryLength_, cellBuffer_Size_, cellOfDefragmentation_Size_, initDoneWithNoErrors_)
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

void* OPTadaC_MultithreadedSimpleMemoryBuffer::GetMemory(size_t new_Length_)
{
	EnterCriticalSection(&threadSynchronization); // запрет на доступ (для синхронизации)
	void* newLink = TakeMemoryMethod(new_Length_);
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
	if (buffer_Length > 0) {
		return (cellBuffer->Get_AllCapturedMemory() + buffer_Length + sizeof(OPTadaC_MultithreadedSimpleMemoryBuffer));
	}
	else {
		return 0;
	}
}