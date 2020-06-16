// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaC_SimpleMemoryBuffer.h"


// Class - standard memory buffer with security data for multithreading
// Upon request, it tries to find a cell with sufficient volume, separates the necessary piece from it and gives it to the user
// Has automatic defragmentation tools for free areas
class OPTadaC_MultithreadedSimpleMemoryBuffer : public OPTadaC_SimpleMemoryBuffer
{
protected:

	CRITICAL_SECTION threadSynchronization; // critical section for thread synchronization

public:

	// The memory will be requested from the OS, an additional buffer will be created
	// [in]   size_t memoryLength_               // The size of the created buffer (bytes)
	// [in]   size_t cellBuffer_Size_            // Size of additional buffer elements (number of elements)
	// [in]   size_t cellOfDefragmentation_Size_ // Cell size (division ratio) to reduce fragmentation
	// [out]  bool&  initDoneWithNoErrors        // to verify the creation of the buffer
	// [CALL] : OPTadaC_SimpleMemoryBuffer(size_, elem_Buffer_Size_, cell_Size_, initDoneWithNoErrors_);
	OPTadaC_MultithreadedSimpleMemoryBuffer(size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_, bool& initDoneWithNoErrors_);

	virtual ~OPTadaC_MultithreadedSimpleMemoryBuffer();


	// !- memory is marked free and does not call destructors -!
	// The method clears all the memory in the buffer for further work with it again
	// return = true - successful | false - error
	bool Clear_Buffer();


	// !- not initialized, only memory allocated -!
	// The method will try to find the closest piece of memory and return it (allocated for use) not an array
	// [in] size_t& new_Length_ // requested memory length (in bytes)
	// return = returns a pointer to memory | nullptr - if failed
	inline void* GetMemory(size_t& new_Length_);

	// !- the destructor is not called, the memory is marked as "free" -!
	// The method will mark the specified memory location as "free"
	// [in] void* link_ // Memory pointer to free
	// return = true - successful | false - error
	inline bool ReturnMemory(void* link_);


	// The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	// return = number - the amount of memory used by the system and its components (in bytes)
	size_t Get_AllModulesLockedMemory();
};