// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTada_MemoryManager.h"


OPTadaS_Key_MemoryManager* OPTada_MemoryManager::initKeyForBuffer(int bufferNomber_)
{
	// get memory for key
	OPTadaS_Key_MemoryManager* newKey = (OPTadaS_Key_MemoryManager*)malloc(sizeof(OPTadaS_Key_MemoryManager));
	if (!newKey) {
		return nullptr;
	}

	// init key 
	newKey->OPTadaS_Key_MemoryManager::OPTadaS_Key_MemoryManager();
	newKey->bufferNomber = bufferNomber_;
	newKey->memoryManagerSaveLink = this;

	return newKey;
}


OPTada_MemoryManager::~OPTada_MemoryManager()
{
	Free_Manager();
}


bool OPTada_MemoryManager::Init_Mamager(int countOfBuffers_)
{
	if (buffer_OfMemoryBuffers) {
		return false;
	}

	if (countOfBuffers_ < 1) {
		return false;
	}

	InitializeCriticalSection(&threadSynchronization); // critical section initialization

	// get memory for buffer list
	buffer_OfMemoryBuffers = (OPTadaS_TemplateMemoryBuffer_Element*)malloc(sizeof(OPTadaS_TemplateMemoryBuffer_Element) * countOfBuffers_); 
	if (buffer_OfMemoryBuffers == nullptr) {
		return false;
	}
	count_OfMemoryBuffers = countOfBuffers_;
	count_createdBuffers = 0;
	
	// init templates
	OPTadaS_TemplateMemoryBuffer_Element* new_elem_ = nullptr;
	for (int i = 0; i < count_OfMemoryBuffers; i++) {

		new_elem_ = &buffer_OfMemoryBuffers[i];
		new_elem_->OPTadaS_TemplateMemoryBuffer_Element::OPTadaS_TemplateMemoryBuffer_Element();
	}	

	return true;
}

bool OPTada_MemoryManager::Free_Manager()
{
	EnterCriticalSection(&threadSynchronization); // access lock (for synchronization)

	OPTadaS_TemplateMemoryBuffer_Element* elem_ = nullptr;
	for (int i = 0; i < count_OfMemoryBuffers; i++) {

		elem_ = &buffer_OfMemoryBuffers[i];

		if (elem_->isLocked == true) {
			if (elem_->buffer != nullptr) {
				elem_->buffer->Clear_Buffer();
				elem_->buffer->~OPTadaC_TemplateOfMemoryBuffer(); // destroy buffer (virtual)
				free(elem_->buffer);
				elem_->buffer = nullptr;
			}
			elem_->bufferID = -1;
			elem_->isLocked = false;
		}
	}

	count_OfMemoryBuffers = 0;
	count_createdBuffers = 0;

	if (buffer_OfMemoryBuffers) {
		free(buffer_OfMemoryBuffers);
		buffer_OfMemoryBuffers = nullptr;
	}

	LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream

	DeleteCriticalSection(&threadSynchronization); // delete critical section

	return true;
}


OPTadaS_Key_MemoryManager* OPTada_MemoryManager::CreateNewMemoryBuffer(int bufferID_, size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_, OPTadaE_BufferTypes_ForMemoryManager bufferType_)
{
	// transferring repeating pieces of code from this method to separate ones is possible
	// I didn’t do this because of the uncertainty regarding initialization of possibly added memory buffers

	EnterCriticalSection(&threadSynchronization); // access lock (for synchronization)

	OPTadaS_TemplateMemoryBuffer_Element* elem_ = nullptr;

	if (count_OfMemoryBuffers - count_createdBuffers > 0 && buffer_OfMemoryBuffers) {	// we can create new buffer

		for (int i = 0; i < count_OfMemoryBuffers; i++) {

			elem_ = &buffer_OfMemoryBuffers[i];
			if (elem_->isLocked == false) { // we can use this element

				// switch bufferType_
				switch (bufferType_)
				{

				case ENUM_SimpleMemoryBuffer: {
					
					// get memory for buffer
					OPTadaC_SimpleMemoryBuffer* newMemoryBuffer = (OPTadaC_SimpleMemoryBuffer*)malloc(sizeof(OPTadaC_SimpleMemoryBuffer));
					if (!newMemoryBuffer) {
						LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
						return nullptr;
					}

					// init memory buffer
					bool initDoneWithNoErrors_ = false;
					newMemoryBuffer->OPTadaC_SimpleMemoryBuffer::OPTadaC_SimpleMemoryBuffer(memoryLength_, cellBuffer_Size_, cellOfDefragmentation_Size_, initDoneWithNoErrors_);
					if (!initDoneWithNoErrors_ || !newMemoryBuffer->TestBuffer()) {
						if (newMemoryBuffer) {
							free(newMemoryBuffer);
						}
						LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
						return nullptr;
					}

					// setup memoryBuffer_element
					elem_->isLocked = true;
					elem_->bufferID = bufferID_;
					elem_->buffer = newMemoryBuffer;

					// create key for buffer
					OPTadaS_Key_MemoryManager* newKey = initKeyForBuffer(i);
					if (!newKey) {
						if (newMemoryBuffer) {
							free(newMemoryBuffer);
						}
						LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
						return nullptr;
					}

					count_createdBuffers++;
					return newKey;

				} break;

				case ENUM_MultithreadedSimpleMemoryBuffer: {

					// get memory for buffer
					OPTadaC_MultithreadedSimpleMemoryBuffer* newMemoryBuffer = (OPTadaC_MultithreadedSimpleMemoryBuffer*)malloc(sizeof(OPTadaC_MultithreadedSimpleMemoryBuffer));
					if (!newMemoryBuffer) {
						LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
						return nullptr;
					}

					// init memory buffer
					bool initDoneWithNoErrors_ = false;
					newMemoryBuffer->OPTadaC_MultithreadedSimpleMemoryBuffer::OPTadaC_MultithreadedSimpleMemoryBuffer(memoryLength_, cellBuffer_Size_, cellOfDefragmentation_Size_, initDoneWithNoErrors_);
					if (!initDoneWithNoErrors_ || !newMemoryBuffer->TestBuffer()) {
						if (newMemoryBuffer) {
							free(newMemoryBuffer);
						}
						LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
						return nullptr;
					}

					// setup memoryBuffer_element
					elem_->isLocked = true;
					elem_->bufferID = bufferID_;
					elem_->buffer = newMemoryBuffer;

					// create key for buffer
					OPTadaS_Key_MemoryManager* newKey = initKeyForBuffer(i);
					if (!newKey) {
						if (newMemoryBuffer) {
							free(newMemoryBuffer);
						}
						LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
						return nullptr;
					}

					count_createdBuffers++;
					return newKey;

				} break;

				default: {
					LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream
					return nullptr;
				} break;

				} // switch end
			}
		}
	}

	LeaveCriticalSection(&threadSynchronization); // allowed access to the next stream

	return nullptr;
}

bool OPTada_MemoryManager::DeleteMemoryBuffer(OPTadaS_Key_MemoryManager** key_Buffer_)
{
	// check key
	if (!(*key_Buffer_) || (*key_Buffer_)->memoryManagerSaveLink != this) {
		return false;
	}

	OPTadaS_TemplateMemoryBuffer_Element* elem_ = &buffer_OfMemoryBuffers[(*key_Buffer_)->bufferNomber];
	if (elem_ != nullptr && elem_->buffer != nullptr) {
		void* dellElem = elem_->buffer;
		((OPTadaC_TemplateOfMemoryBuffer*)dellElem)->Clear_Buffer();
		((OPTadaC_TemplateOfMemoryBuffer*)dellElem)->~OPTadaC_TemplateOfMemoryBuffer(); // destroy buffer (virtual)
		free(dellElem);
		elem_->buffer = nullptr;
		elem_->bufferID = -1;
		elem_->isLocked = false;
		count_createdBuffers--;
	}

	// kill key
	if ((*key_Buffer_)) {
		free((*key_Buffer_)); // !WARNING!
	}
	(*key_Buffer_) = nullptr;

	return true;
}

bool OPTada_MemoryManager::Clear_Buffer(OPTadaS_Key_MemoryManager* key_Buffer_)
{

#ifdef OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY
	// check key
	if (!key_Buffer_ || key_Buffer_->memoryManagerSaveLink != this) {
		return false;
	}
#endif // OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY

	return (&buffer_OfMemoryBuffers[key_Buffer_->bufferNomber])->buffer->Clear_Buffer();
}


void* OPTada_MemoryManager::GetMemory(OPTadaS_Key_MemoryManager* key_Buffer_, size_t size_)
{

#ifdef OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY
	// check key
	if (!key_Buffer_ || key_Buffer_->memoryManagerSaveLink != this) {
		return nullptr;
	}
#endif // OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY

	return (&buffer_OfMemoryBuffers[key_Buffer_->bufferNomber])->buffer->GetMemory(size_);
}

bool OPTada_MemoryManager::ReturnMemory(OPTadaS_Key_MemoryManager* key_Buffer_, void* link_)
{

#ifdef OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY
	// check key
	if (!key_Buffer_ || key_Buffer_->memoryManagerSaveLink != this) {
		return false;
	}
#endif // OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY

	return (&buffer_OfMemoryBuffers[key_Buffer_->bufferNomber])->buffer->ReturnMemory(link_);
}


size_t OPTada_MemoryManager::Get_LockedMemory(OPTadaS_Key_MemoryManager* key_Buffer_)
{

#ifdef OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY
	// check key
	if (!key_Buffer_ || key_Buffer_->memoryManagerSaveLink != this) {
		return false;
	}
#endif // OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY

	return (&buffer_OfMemoryBuffers[key_Buffer_->bufferNomber])->buffer->Get_LockedMemory();
}

size_t OPTada_MemoryManager::Get_BufferMemorySize(OPTadaS_Key_MemoryManager* key_Buffer_)
{

#ifdef OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY
	// check key
	if (!key_Buffer_ || key_Buffer_->memoryManagerSaveLink != this) {
		return false;
	}
#endif // OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY

	return (&buffer_OfMemoryBuffers[key_Buffer_->bufferNomber])->buffer->Get_BufferMemorySize();
}

size_t OPTada_MemoryManager::Get_AllModulesLockedMemory()
{
	size_t allCapturedMemory = 0;

	// me 
	allCapturedMemory += sizeof(OPTada_MemoryManager) + (sizeof(OPTadaS_TemplateMemoryBuffer_Element) * count_OfMemoryBuffers);

	// all buffers
	for (int i = 0; i < count_OfMemoryBuffers; i++) {

		if ((&buffer_OfMemoryBuffers[i])->isLocked == true) { // we have buffer here
			allCapturedMemory += (&buffer_OfMemoryBuffers[i])->buffer->Get_AllModulesLockedMemory();
		}
	}

	return allCapturedMemory;
}
