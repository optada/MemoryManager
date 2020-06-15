// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once


// structure - element for creating linked lists of memory cells (56 Byte)
struct OPTadaS_MemoryCell_Element
{
public:

	size_t size   = 0;       // buffer length in bytes (0 - buffer is free)
	char*  link   = nullptr; // memory pointer
	bool   isfree = true;    // variable designating memory state

	OPTadaS_MemoryCell_Element* next_el     = nullptr; // pointer to next element
	OPTadaS_MemoryCell_Element* previous_el = nullptr; // pointer to previous element

	OPTadaS_MemoryCell_Element* next_link     = nullptr; // pointer to next element (second buffer)
	OPTadaS_MemoryCell_Element* previous_link = nullptr; // pointer to previous element (second buffer)
};
