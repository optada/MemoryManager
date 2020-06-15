// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_MemoryCells_StaticCyclicBuffer.h"


OPTadaC_MemoryCells_StaticCyclicBuffer::OPTadaC_MemoryCells_StaticCyclicBuffer(size_t size_, bool& initDoneWithNoErrors_)
{
	buffer = (OPTadaS_MemoryCell_Element*)malloc(sizeof(OPTadaS_MemoryCell_Element) * size_);
	length = size_;
	locked = 0;
	index = 0;

	if (buffer != nullptr) {
		for (int i = 0; i < length; i++) // use constructor for all cell structures
			buffer[i] = OPTadaS_MemoryCell_Element();
		buffer_last = &buffer[length - 1];
		initDoneWithNoErrors_ = true;
	}
	else {
		buffer_last = nullptr;
		length = 0;
		index = 0;
		initDoneWithNoErrors_ = false; // can not get memory for buffer
	}
}

OPTadaC_MemoryCells_StaticCyclicBuffer::~OPTadaC_MemoryCells_StaticCyclicBuffer()
{
	if (buffer != nullptr) {
		free(buffer);
		buffer = nullptr;
	}
}

OPTadaS_MemoryCell_Element* OPTadaC_MemoryCells_StaticCyclicBuffer::Get_Element()
{
	if (length > locked) { // we have free cell
		do {
			// we are looking for a free cell
			if (buffer[index].size == 0) { // cell is free
				OPTadaS_MemoryCell_Element* FElem = &buffer[index];
				FElem->size = 1;
				FElem->isfree = true;
				locked++;

				return FElem; // free cell ready
			}
			else { // cell is locked
				index++;
				if (index >= length) { // search cycle check
					index = 0;
				}
			}
		} while (true);
	}
	else { // we have no free cell
		return nullptr; 
	}
}

bool OPTadaC_MemoryCells_StaticCyclicBuffer::Return_Element(OPTadaS_MemoryCell_Element* elem_)
{
	if (elem_ != nullptr && (elem_ >= buffer && elem_ <= buffer_last)) { // there is such a link
		elem_->size = 0; // cell is free now
		locked--;

		return true;
	}
	else { // nullptr - there is no such cell in this array
		return false;
	}
}

size_t OPTadaC_MemoryCells_StaticCyclicBuffer::Get_AllCapturedMemory()
{
	return (sizeof(OPTadaC_MemoryCells_StaticCyclicBuffer) + sizeof(OPTadaS_MemoryCell_Element) * length);
}