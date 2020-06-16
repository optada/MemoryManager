// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "windows.h" // to synchronize threads + (malloc|free)

#include "CellBuffer\OPTadaC_MemoryCells_StaticCyclicBuffer.h"


// class - template for various memory buffers
class OPTadaC_TemplateOfMemoryBuffer
{
protected:

	char*  buffer        = nullptr; // pointer to memory buffer
	size_t buffer_Length = 0;       // total size of memory in this buffer (in bytes)
	size_t lockedMemory  = 0;       // size of memory occupied in this buffer (in bytes)

public:

	virtual ~OPTadaC_TemplateOfMemoryBuffer() = 0 {};

	// virtual | The method clears all the memory in the buffer for further work with it again | !- The memory is marked free and does not call destructors -!
	virtual bool Clear_Buffer() = 0; 


	// virtual | The method will try to find the closest cell of memory and return it (give for use)
	virtual inline void* GetMemory(size_t& new_Length_) = 0; 

	// virtual | The method will mark the specified memory location as "free"
	virtual inline bool ReturnMemory(void* link_) = 0;


	// virtual | The method tests the buffer for errors when creating
	virtual bool TestBuffer() = 0; 

	// virtual | The method returns the amount of occupied memory in the buffer (in bytes)
	virtual size_t Get_LockedMemory() = 0; 

	// virtual | The method returns the size of the memory buffer (in bytes)
	virtual size_t Get_BufferMemorySize() = 0;

	// virtual | The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	virtual size_t Get_AllModulesLockedMemory() = 0; 
};