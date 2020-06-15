// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTada_MemoryManager.h"


bool OPTada_MemoryManager::Init_Mamager(int Size_buff_)
{
	InitializeCriticalSection(&threadSynchronization); // critical section initialization

	if (Size_buff_ < 1) {
		return false;
	}
	Size_buff_ += 1;

	// get memory for buffer list
	buffer_OfMemoryBuffers = (OPTadaS_TemplateMemoryBuffer_Element*)malloc(sizeof(OPTadaS_TemplateMemoryBuffer_Element) * Size_buff_); 
	if (buffer_OfMemoryBuffers == nullptr) {
		return false;
	}
	size_buffer_OfMemoryBuffers = Size_buff_;
	
	// init templates
	OPTadaS_TemplateMemoryBuffer_Element * new_elem_ = nullptr;
	for (int i = 0; i < size_buffer_OfMemoryBuffers; i++) {

		new_elem_ = &buffer_OfMemoryBuffers[i];
		new_elem_->OPTadaS_TemplateMemoryBuffer_Element::OPTadaS_TemplateMemoryBuffer_Element();
	}	

	return true;
}

bool OPTada_MemoryManager::Free_Manager()
{
	DeleteCriticalSection(&threadSynchronization); // delete critical section

	OPTadaS_TemplateMemoryBuffer_Element* elem_ = nullptr;
	for (int i = 0; i < size_buffer_OfMemoryBuffers; i++) {

		elem_ = &buffer_OfMemoryBuffers[i];

		if (elem_->isLocked == true) {
			if (elem_->buffer != nullptr) {
				elem_->buffer->Clear_Buffer();
				//elem_->buffer->~OPTada_Memory_C_TemplateMemoryBuffer(); //TODO ыыы // не корректный вызов???
				free(elem_->buffer);
			}
			elem_->bufferID = -1;
			elem_->isLocked = false;
		}
	}

	size_buffer_OfMemoryBuffers = 0;
	if (buffer_OfMemoryBuffers) {
		free(buffer_OfMemoryBuffers);
	}

	return true;
}

OPTadaS_MemoryManedger* OPTada_MemoryManager::CreateNewMemoryBuffer(int type_, size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_)
{
	OPTadaS_TemplateMemoryBuffer_Element * elem_ = nullptr;
	for (int i = 0; i < size_buffer_OfMemoryBuffers; i++)
	{
		elem_ = &buffer_OfMemoryBuffers[i];
		if (elem_->isLocked == false)
		{
			elem_->isLocked = true;
			elem_->bufferID = type_; //TODO не тип а ID!
			switch (type_) // type
			{
			case 1:
			{
				// создание и сразу возврат указателя + не забыть освободить синхронизацию потоков
			}
				break;
			case 2:
			{
				// создание многопоточного буфера и сразу возврат указателя + не забыть освободить синхронизацию потоков
			}
				break;
			default:
			{
				elem_->bufferID = 0;
				elem_->buffer = nullptr;
				elem_->isLocked = false;
				return nullptr;
			}
				break;
			}
		}
	}
	return nullptr;
}
