// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaC_TemplateOfMemoryBuffer.h"


// class - standard memory buffer
// Upon request, it tries to find a cell with sufficient volume, separates the necessary piece from it and gives it to the user
// Has automatic defragmentation tools for free areas
class OPTadaC_SimpleMemoryBuffer : public OPTadaC_TemplateOfMemoryBuffer
{
protected:

	OPTadaC_MemoryCells_StaticCyclicBuffer* cellBuffer = nullptr; // Buffer with elements - cells

	OPTadaS_MemoryCell_Element* firstCell_Buffer   = nullptr; // Pointer to the first cell
	OPTadaS_MemoryCell_Element* freeCells_Buffer   = nullptr; // Free Cell Buffer
	OPTadaS_MemoryCell_Element* lockedCells_Buffer = nullptr; // Locked Cell Buffer

	size_t cellOfDefragmentation_Size = 0; // coefficient of division of information (to reduce fragmentation) (byte)


	// The method searches for free memory, if found, does all the work and allocates
	inline void* TakeMemoryMethod(size_t& new_size_);

	// The method cuts a cell from the buffer
	// [in] OPTadaS_MemoryCell_Element* cell_elem_  // link on cell element
	// [in] OPTadaS_MemoryCell_Element** buffer_    // target buffer
	inline void CutCellFromTheBuffer(OPTadaS_MemoryCell_Element* cell_elem_, OPTadaS_MemoryCell_Element** buffer_);

	// transfer cell to buffer
	// [in] OPTadaS_MemoryCell_Element* cell_elem_  // link on cell element
	// [in] OPTadaS_MemoryCell_Element** buffer_    // target buffer
	inline void TransferCellToBuffer(OPTadaS_MemoryCell_Element* cell_elem_, OPTadaS_MemoryCell_Element** buffer_);

	// The method tries to merge neighboring free cells
	// [in] OPTadaS_MemoryCell_Element* cell_elem_ // link on cell element
	// return = true - merge successfull | false - no merger
	inline bool TryMergeNeighboringFreeCells(OPTadaS_MemoryCell_Element* cell_elem_);

public:

	// The memory will be requested from the OS, an additional buffer will be created
	// [in]  size_t memoryLength_               // The size of the created buffer (bytes)
	// [in]  size_t cellBuffer_Size_            // Size of additional buffer elements (number of elements)
	// [in]  size_t cellOfDefragmentation_Size_ // Cell size (division ratio) to reduce fragmentation
	// [out] bool&  initDoneWithNoErrors        // to verify the creation of the buffer
	OPTadaC_SimpleMemoryBuffer(size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_, bool& initDoneWithNoErrors_);

	virtual ~OPTadaC_SimpleMemoryBuffer();


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


	// The method tests the buffer for errors when creating
	// return = true - the buffer is successful | false - creating buffer failed or with mistakes
	bool TestBuffer();

	// The method returns the count of locked memory in the buffer (in bytes)
	// return = count of memory used (locked) in this buffer
	size_t Get_LockedMemory();

	// The method returns the size of the memory buffer (in bytes)
	// return = number - buffer size(in bytes) | 0 - buffer not created / error
	size_t Get_BufferMemorySize();

	// The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	// return = number - the amount of memory used by the system and its components (in bytes)
	size_t Get_AllModulesLockedMemory();
};