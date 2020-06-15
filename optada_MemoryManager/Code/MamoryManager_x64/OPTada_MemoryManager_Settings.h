#pragma once


//#include "windows.h" // для синхронизации потоков
//#define USE_RELOAD_NEW_BUFFER

#define OPTada_Memory_DEF_StandartSizeNewBuffer 100         // размер создаваемого нулевого буфера (для new) 
#define OPTada_Memory_DEF_StandartSizeNewBufferElems 10     // размер создаваемого нулевого буфера (для new) 
#define OPTada_Memory_DEF_StandartSizeNewBufferCellSize 1   // размер создаваемого нулевого буфера (для new) 

#ifdef USE_RELOAD_NEW_BUFFER
	
#endif // USE_RELOAD_NEW_BUFFER


//#include <thread>
//#include "process.h"