// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_MultithreadedSimpleMemoryBuffer.h"


OPTadaC_MultithreadedSimpleMemoryBuffer::OPTadaC_MultithreadedSimpleMemoryBuffer(size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_, bool& initDoneWithNoErrors_) 
	: OPTadaC_SimpleMemoryBuffer(memoryLength_, cellBuffer_Size_, cellOfDefragmentation_Size_, initDoneWithNoErrors_)
{
	InitializeCriticalSection(&threadSynchronization); // critical section initialization
}

OPTadaC_MultithreadedSimpleMemoryBuffer::~OPTadaC_MultithreadedSimpleMemoryBuffer()
{
	DeleteCriticalSection(&threadSynchronization); // critical section delete
}


bool OPTadaC_MultithreadedSimpleMemoryBuffer::Clear_Buffer()
{
	EnterCriticalSection(&threadSynchronization); // access lock (for synchronization)
	bool returnResult = OPTadaC_SimpleMemoryBuffer::Clear_Buffer();
	LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
	return returnResult;
}


inline void* OPTadaC_MultithreadedSimpleMemoryBuffer::GetMemory(size_t& new_Length_)
{
	EnterCriticalSection(&threadSynchronization); // access lock (for synchronization)
	void* newLink = TakeMemoryMethod(new_Length_);
	LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
	return newLink;
}

inline bool OPTadaC_MultithreadedSimpleMemoryBuffer::ReturnMemory(void* link_)
{
	EnterCriticalSection(&threadSynchronization); // access lock (for synchronization)
	bool returnResult = OPTadaC_SimpleMemoryBuffer::ReturnMemory(link_);
	LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
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