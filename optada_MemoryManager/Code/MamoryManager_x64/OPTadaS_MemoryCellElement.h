// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once


// structure - element for creating linked lists of memory cells (56 Byte)
struct OPTadaS_MemoryCellElement
{
public:
	size_t size   = 0;    // buffer length in bytes (0 - buffer is free)
	char*  link   = NULL; // memory pointer
	bool   isfree = true; // variable designating memory state

	OPTadaS_MemoryCellElement* next_el     = NULL; // pointer to next element
	OPTadaS_MemoryCellElement* previous_el = NULL; // pointer to previous element

	OPTadaS_MemoryCellElement* next_link     = NULL; // pointer to next element (second buffer)
	OPTadaS_MemoryCellElement* previous_link = NULL; // pointer to previous element (second buffer)

	OPTadaS_MemoryCellElement();
};

OPTadaS_MemoryCellElement::OPTadaS_MemoryCellElement()
{
	size = 0;
}