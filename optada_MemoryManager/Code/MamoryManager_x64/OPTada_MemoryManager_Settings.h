// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once


// Use it, if you need more safety in Get\return memory methods
//#define OPTada_IFDEF_MEMORYMANAGER_USE_MORE_SAFETY


// Use it for reload New() and Delete()
//#define USE_RELOAD_NEW_DELETE

#ifdef USE_RELOAD_NEW_DELETE


#define OPTada_Memory_DEF_StandartSizeNewBuffer 100000      // размер создаваемого нулевого буфера (для new) 
#define OPTada_Memory_DEF_StandartSizeNewCellBuffer 100000  // размер создаваемого нулевого буфера (для new) 
#define OPTada_Memory_DEF_StandartSizeNewBufferCellSize 1   // размер создаваемого нулевого буфера (для new) 

#endif // USE_RELOAD_NEW_DELETE
