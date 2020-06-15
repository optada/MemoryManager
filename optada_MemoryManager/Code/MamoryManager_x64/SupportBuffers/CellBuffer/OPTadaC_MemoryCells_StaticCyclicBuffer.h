// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "windows.h" // (malloc|free)

#include "OPTadaS_MemoryCell_Element.h"


// class - cyclic buffer storing cells OPTadaS_MemoryCellElement (to create linked memory manager buffer lists)
class OPTadaC_MemoryCells_StaticCyclicBuffer
{
private:

	OPTadaS_MemoryCell_Element* buffer      = nullptr; // element buffer
	OPTadaS_MemoryCell_Element* buffer_last = nullptr; // pointer to the beginning of the last element in the buffer

	size_t length = 0; // total number of cells
	size_t locked = 0; // number of captured cells
	size_t index  = 0; // pointer to the current element in the buffer (on the free, or 0)	

public:

	// [in]  size_t size_                // buffer Size - Number of Cells Created
	// [out] bool&  initDoneWithNoErrors // to verify the creation of the buffer
	OPTadaC_MemoryCells_StaticCyclicBuffer(size_t size_, bool& initDoneWithNoErrors_);

	~OPTadaC_MemoryCells_StaticCyclicBuffer();


	// Request for a free item
	// return = link on the element | nullptr - can not take element (error)
	OPTadaS_MemoryCell_Element* Get_Element();

	// Returns an item (free it) !- trash remains in the cell -!
	// [in] OPTadaS_MemoryCell_Element* elem_ // Element pointer
	// return = true - done | false - error
	bool Return_Element(OPTadaS_MemoryCell_Element* elem_);

	// Returns the amount of memory consumed by this class (+ captured memory)
	// return = Returns the amount of memory consumed by this class (+ captured memory) (in bytes)
	size_t Get_AllCapturedMemory();
};