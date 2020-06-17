HI! My nick is - OPTada. This project - (Memory manager), is my pet-project. 
GitHub   : https://github.com/optada/MemoryManager

my mail@ : druzhinin.optada@gmail.com

// --------------------------------------------------------------------------------------------------
// Description

This project implements functionality that allows you to pre-capture memory and use it directly.
The project implements a concept that allows to speed up the process of requesting memory 
by capturing it in advance.


// --------------------------------------------------------------------------------------------------
// Functional

OPTada_MemoryMeneger - a class that provides memory management functionality.
It must be initialized before use.
This class contains the mechanisms of creation \ control \ destruction for classes -
memory buffers. All memory buffer classes must based on OPTadaC_TemplateOfMemoryBuffer
to preserve polymorphism and the overall concept of memory manager automation.
The problems of memory fragmentation are solved by automatic merging of neighboring free memory areas.

OPTada_MemoryMeneger::Functional :
- Create a new memory buffer
- Removing the memory buffer
- Cross memory chunk (like a new() - without constructor)
- Return (release) the occupied piece of memory (like a delete() - without destructor)
- Pass information about memory buffers (size, locked memory, all occupied memory)

project for WIN_x64, but you can rebild it easy to WIN_x32

The project does not affect the functionality of (new | delete) and also (malloc | realloc | free)
the integration is painless and easy.


// --------------------------------------------------------------------------------------------------
// Architecture

#include windows.h 
(using malloc|free) + (thread synchronization)

OPTada_MemoryMenager - Uses memory buffers inherited from OPTadaC_TemplateOfMemoryBuffer


OPTadaC_TemplateOfMemoryBuffer - Used as an interface class to create memory buffers with different
internal logic.

OPTadaC_SimpleMemoryBuffer - Simple class with automatic defragmentation

OPTadaC_MultithreadedSimpleMemoryBuffer - A simple class with automatic defragmentation and multi-threaded protection


OPTadaC_MemoryCells_StaticCyclicBuffer - Class for querying / deleting memory cells. Used for optimization
in memory buffer classes. Gives cells that can refer to specific sections of memory.


// --------------------------------------------------------------------------------------------------
// Integrate 

To integrate the memory manager, follow the instructions:
1) put the folder MemoryManager_x64 in your project. (optada_memoryManager --> code --> )
2) Include the OPTada_MemoryManager.h header file
3) Create an OPTada_MemoryMenager object
4) Initialize the OPTada_MemoryMenager object
5) Create a memory buffer in OPTada_MemoryMenager and save OPTadaS_Key_MemoryManager to it
6) Pass OPTadaS_Key_MemoryManager to access the buffer in OPTada_MemoryMenager

! - Upon completion, use OPTada_MemoryMenager :: Free_Manager (); -!


// --------------------------------------------------------------------------------------------------
// Usage tips

This project implements a memory request system in ideal conditions up to 70% faster than new () | delete ()
Depending on the amount of data and the amount of fragmentation, this indicator may deteriorate.
It is recommended to use this memory manager if there is a need for data to be completely overwritten,
or when using large, same type element buffers. However, with random requests
its speed is on average faster than the standard new | delete up to 10,000 unique elements.
The problems of memory fragmentation are solved by automatic merging of neighboring free memory areas.
To add your own memory processing logic, use OPTadaC_TemplateOfMemoryBuffer as an interface.
Create and query memory chunks in advance.


// --------------------------------------------------------------------------------------------------
If you find a mistake|bug|error, please describe it and write to me:
on my mail@ : druzhinin.optada@gmail.com
or GitHub   : https://github.com/optada/MemoryManager
