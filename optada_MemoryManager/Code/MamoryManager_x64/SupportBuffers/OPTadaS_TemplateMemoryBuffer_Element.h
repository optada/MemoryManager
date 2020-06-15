// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaC_TemplateOfMemoryBuffer.h"


// structure for memory manager
struct OPTadaS_TemplateMemoryBuffer_Element
{
public:

	OPTadaC_TemplateOfMemoryBuffer* buffer = nullptr; // reference to a template class (element - buffer)
	bool isLocked	                       = false;   // false - cell is free | true - blocked (busy)
	int bufferID                           = -1;      // ID for buffer (you can set this information)
};