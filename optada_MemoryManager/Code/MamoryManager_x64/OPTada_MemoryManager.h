// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTada_MemoryManager_Settings.h"

#include "SupportBuffers\OPTadaS_TemplateMemoryBuffer_Element.h"

#include "SupportBuffers\OPTadaC_TemplateOfMemoryBuffer.h"
#include "SupportBuffers\OPTadaC_SimpleMemoryBuffer.h"
#include "SupportBuffers\OPTadaC_MultithreadedSimpleMemoryBuffer.h"


// structure for accessing the memory buffer
// !- DO NOT CHANGE DATA IN THIS STRUCTURE -!
struct OPTadaS_Key_MemoryManager
{
	int bufferNomber            = -1;      // memory buffer number in the memory manager
	void* memoryManagerSaveLink = nullptr; // link to the memory manager for checking the key
};


// enum list-types of memory buffers
enum OPTadaE_BufferTypes_ForMemoryManager {
	NONE                                 = 0, // -
	ENUM_SimpleMemoryBuffer              = 1, // OPTadaC_SimpleMemoryBuffer.h
	ENUM_MultithreadedSimpleMemoryBuffer = 2, // OPTadaC_MultithreadedSimpleMemoryBuffer.h
};


// MemoryManager
// !- need initialithation -!
// after init - Thread-safety
class OPTada_MemoryManager
{
private:

	OPTadaS_TemplateMemoryBuffer_Element* buffer_OfMemoryBuffers = nullptr; // Buffer cells - elements
	int count_OfMemoryBuffers                                    = 0;       // The number of cells in the buffer_OfMemoryBuffers
	int count_createdBuffers                                     = 0;       // how many buffers was created

	CRITICAL_SECTION threadSynchronization; // critical section for thread synchronization


	// method initializes the access key to the buffer
	// [in] int bufferNomber_ // buffer nomber in mass
	// return = true - link on key | false - nullptr
	OPTadaS_Key_MemoryManager* initKeyForBuffer(int bufferNomber_);

public:

	~OPTada_MemoryManager();


	// initializes the memory manager
	// [in] int countOfBuffers_ // The number of buffers-elements to be created.
	// return = true - successful | false - error
	bool Init_Mamager(int countOfBuffers_);

	// frees all captured memory in the manager (removes all buffers and frees memory)
	// return = true - successful | false - error
	bool Free_Manager();


	// The method creates a new buffer and returns Buffer_MemoryManedger *
	// [in] int bufferID_                                    // you can use it like a indificator
	// [in] size_t memoryLength_                             // The amount of memory in the created buffer
	// [in] size_t cellBuffer_Size_                          // Number of request cells in the created buffer
	// [in] size_t cellOfDefragmentation_Size_               // Cell size (division ratio) to reduce fragmentation
	// [in] OPTadaE_BufferTypes_ForMemoryManager bufferType_ // type of memory buffer
	// return = if successful, will return a pointer to the structure for accessing the created buffer | nullptr - on error
	OPTadaS_Key_MemoryManager* CreateNewMemoryBuffer(int bufferID_, size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_, OPTadaE_BufferTypes_ForMemoryManager bufferType_);

	// !- METHOD WILL KILL YOUR KEY -!
	// method clears and completely deletes the transferred memory buffer
	// [in] OPTadaS_Key_MemoryManager * key_Buffer_ // memory buffer key
	// return = true - успешно | false - ошибка
	bool DeleteMemoryBuffer(OPTadaS_Key_MemoryManager** key_Buffer_);


	// !- memory is marked free and does not call destructors -!
	// The method clears all the memory in the buffer for further work with it again
	// [in] OPTadaS_Key_MemoryManager* key_Buffer_ // memory buffer key
	// return = true - successful | false - error
	bool Clear_Buffer(OPTadaS_Key_MemoryManager* key_Buffer_);


	// The method allocates memory of the requested size (a single piece without additional information)
	// [in] OPTadaS_Key_MemoryManager* key_Buffer_ // memory buffer key
	// [in] size_t size_                            // requested memory
	// return = pointer to memory if successful | nullptr - on error
	void* GetMemory(OPTadaS_Key_MemoryManager* key_Buffer_, size_t size_);

	// !- the destructor is not called, the memory is marked as "free" -!
	// The method will mark the specified memory location as "free"
	// [in] OPTadaS_Key_MemoryManager* key_Buffer_ // Structure to indicate the required buffer
	// [in] void* link_                            // Pointer to memory to be freed
	// return = true - successful | false - error
	bool ReturnMemory(OPTadaS_Key_MemoryManager* key_Buffer_, void* link_);


	// The method returns the count of locked memory in the buffer (in bytes)
	// [in] OPTadaS_Key_MemoryManager* key_Buffer_ // Structure to indicate the required buffer, if nullptr - from all buffers
	// return = count of memory used (locked) in this buffer
	size_t Get_LockedMemory(OPTadaS_Key_MemoryManager* key_Buffer_);

	// The method returns the size of the memory buffer (in bytes)
	// [in] OPTadaS_Key_MemoryManager* key_Buffer_ // Structure to indicate the required buffer, if nullptr - from all buffers
	// return = number - buffer size(in bytes) | 0 - buffer not created / error
	size_t Get_BufferMemorySize(OPTadaS_Key_MemoryManager* key_Buffer_);

	// The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	// return = number - the amount of memory used by the system and its components (in bytes)
	size_t Get_AllModulesLockedMemory();
};
