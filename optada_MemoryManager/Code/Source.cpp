// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;


// #include "MamoryManager_x64\SupportBuffers\CellBuffer\OPTadaC_MemoryCells_StaticCyclicBuffer.h"
// test for OPTadaC_MemoryCells_StaticCyclicBuffer | OK
/*
void testFor_OPTadaC_MemoryCells_StaticCyclicBuffer()
{
	bool del = false;
	OPTadaC_MemoryCells_StaticCyclicBuffer Buffer = OPTadaC_MemoryCells_StaticCyclicBuffer(5, del);

	cout << endl << del;
	cout << endl << Buffer.Get_AllCapturedMemory();
	
	void* element1 = nullptr;
	void* element2 = nullptr;
	void* element3 = nullptr;
	void* element4 = nullptr;
	void* element5 = nullptr;
	void* element6 = nullptr;

	element1 = Buffer.Get_Element();
	element2 = Buffer.Get_Element();
	element3 = Buffer.Get_Element();
	element4 = Buffer.Get_Element();
	element5 = Buffer.Get_Element();
	element6 = Buffer.Get_Element();

	Buffer.Return_Element((OPTadaS_MemoryCell_Element*)element2);

	element2 = Buffer.Get_Element();

	Buffer.Return_Element((OPTadaS_MemoryCell_Element*)element1);
	Buffer.Return_Element((OPTadaS_MemoryCell_Element*)element2);
	Buffer.Return_Element((OPTadaS_MemoryCell_Element*)element3);
	Buffer.Return_Element((OPTadaS_MemoryCell_Element*)element4);
	Buffer.Return_Element((OPTadaS_MemoryCell_Element*)element5);
	Buffer.Return_Element((OPTadaS_MemoryCell_Element*)element6);
}
*/

// #include "MamoryManager_x64\SupportBuffers\OPTadaC_SimpleMemoryBuffer.h"
// test for OPTadaC_SimpleMemoryBuffer | OK
/*
void testFor_OPTadaC_SimpleMemoryBuffer()
{
	bool initDoneWithNoErrors_ = false;
	OPTadaC_SimpleMemoryBuffer Buffer = OPTadaC_SimpleMemoryBuffer(10, 5, 2, initDoneWithNoErrors_);

	cout << "initDoneWithNoErrors_ - " << initDoneWithNoErrors_ << endl;
	cout << "test buffer - " << Buffer.TestBuffer() << endl;
	cout << "All locked memory :" << Buffer.Get_AllModulesLockedMemory() << endl;
	cout << "Memory size :" << Buffer.Get_BufferMemorySize() << endl;
	cout << "Locked memory :" << Buffer.Get_LockedMemory() << endl;

	void* pok1 = nullptr;
	void* pok2 = nullptr;
	void* pok3 = nullptr;
	void* pok4 = nullptr;
	void* pok5 = nullptr;
	void* pok6 = nullptr;

	// incorrect or specifick input
	pok1 = Buffer.GetMemory(-1); // ! unsigned parameter
	pok1 = Buffer.GetMemory(1000000); // fragmetation +0 (2b - fragmet cell)
	pok1 = Buffer.GetMemory(1000001); // fragmetation +1 (2b - fragmet cell)
	
	// incorrect reff
	int a;
	pok1 = &a;
	Buffer.ReturnMemory(pok1);
	pok1 = nullptr;
	Buffer.ReturnMemory(pok1);

	// try get memory errors
	pok1 = Buffer.GetMemory(8);
	// low memory
	pok2 = Buffer.GetMemory(3); // we have 2 now
	// have no enaght memory
	pok3 = Buffer.GetMemory(11); // we have 10 max

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = nullptr;
	pok2 = nullptr;
	pok3 = nullptr;

	// fragmentaion mamory error +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00 
	// now we have 8 memory (2 * 4 cells)
	pok4 = Buffer.GetMemory(8); // error (nullptr)

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = nullptr;
	pok2 = nullptr;
	pok3 = nullptr;
	pok4 = nullptr;

	// marege free memory +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00 
	Buffer.ReturnMemory(pok2); // 00 00-00-00 00

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = nullptr;
	pok2 = nullptr;
	pok3 = nullptr;
	pok4 = nullptr;

}
*/

// #include "MamoryManager_x64\SupportBuffers\OPTadaC_MultithreadedSimpleMemoryBuffer.h"
// test for OPTadaC_MultithreadedSimpleMemoryBuffer | OK
/*
void testFor_OPTadaC_MultithreadedSimpleMemoryBuffer()
{
	bool initDoneWithNoErrors_ = false;
	OPTadaC_MultithreadedSimpleMemoryBuffer Buffer = OPTadaC_MultithreadedSimpleMemoryBuffer(10, 5, 2, initDoneWithNoErrors_);

	cout << "initDoneWithNoErrors_ - " << initDoneWithNoErrors_ << endl;
	cout << "test buffer - " << Buffer.TestBuffer() << endl;
	cout << "All locked memory :" << Buffer.Get_AllModulesLockedMemory() << endl;
	cout << "Memory size :" << Buffer.Get_BufferMemorySize() << endl;
	cout << "Locked memory :" << Buffer.Get_LockedMemory() << endl;

	void* pok1 = nullptr;
	void* pok2 = nullptr;
	void* pok3 = nullptr;
	void* pok4 = nullptr;
	void* pok5 = nullptr;
	void* pok6 = nullptr;

	// incorrect or specifick input
	pok1 = Buffer.GetMemory(-1); // ! unsigned parameter
	pok1 = Buffer.GetMemory(1000000); // fragmetation +0 (2b - fragmet cell)
	pok1 = Buffer.GetMemory(1000001); // fragmetation +1 (2b - fragmet cell)

	// incorrect reff
	int a;
	pok1 = &a;
	Buffer.ReturnMemory(pok1);
	pok1 = nullptr;
	Buffer.ReturnMemory(pok1);

	// try get memory errors
	pok1 = Buffer.GetMemory(8);
	// low memory
	pok2 = Buffer.GetMemory(3); // we have 2 now
	// have no enaght memory
	pok3 = Buffer.GetMemory(11); // we have 10 max

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = nullptr;
	pok2 = nullptr;
	pok3 = nullptr;

	// fragmentaion mamory error +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00
	// now we have 8 memory (2 * 4 cells)
	pok4 = Buffer.GetMemory(8); // error (nullptr)

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = nullptr;
	pok2 = nullptr;
	pok3 = nullptr;
	pok4 = nullptr;

	// marege free memory +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00
	Buffer.ReturnMemory(pok2); // 00 00-00-00 00

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = nullptr;
	pok2 = nullptr;
	pok3 = nullptr;
	pok4 = nullptr;

}
*/

#include "MamoryManager_x64\OPTada_MemoryManager.h"
// test for OPTada_MemoryMeneger | OK
/*
void testFor_OPTada_MemoryManager()
{
	void* point1 = nullptr;
	void* point2 = nullptr;
	void* point3 = nullptr;
	void* point4 = nullptr;
	void* point5 = nullptr;
	void* point6 = nullptr;

	OPTadaS_Key_MemoryManager* key1 = nullptr;
	OPTadaS_Key_MemoryManager* key2 = nullptr;
	OPTadaS_Key_MemoryManager* key3 = nullptr;
	OPTadaS_Key_MemoryManager* key4 = nullptr;
	OPTadaS_Key_MemoryManager* key5 = nullptr;
	OPTadaS_Key_MemoryManager* key6 = nullptr;

	OPTada_MemoryManager manager;
	OPTada_MemoryManager manager2;

	// init, free, new buffer, delete buffer, get memory, create key, check key
	manager.Init_Mamager(3);
	manager2.Init_Mamager(1);

	key1 = manager.CreateNewMemoryBuffer(13, 500000000, 5, 5, OPTadaE_BufferTypes_ForMemoryManager::ENUM_MultithreadedSimpleMemoryBuffer);
	key2 = manager.CreateNewMemoryBuffer(12, 500000000, 5, 5, OPTadaE_BufferTypes_ForMemoryManager::ENUM_SimpleMemoryBuffer);
	key3 = manager.CreateNewMemoryBuffer(11, 500000000, 5, 5, OPTadaE_BufferTypes_ForMemoryManager::ENUM_MultithreadedSimpleMemoryBuffer);

	key4 = manager2.CreateNewMemoryBuffer(11, 1000000, 5, 5, OPTadaE_BufferTypes_ForMemoryManager::ENUM_MultithreadedSimpleMemoryBuffer);
	key5 = manager2.CreateNewMemoryBuffer(11, 1000000, 5, 5, OPTadaE_BufferTypes_ForMemoryManager::ENUM_MultithreadedSimpleMemoryBuffer);
	manager2.DeleteMemoryBuffer(&key4);
	key5 = manager2.CreateNewMemoryBuffer(11, 1000000, 5, 5, OPTadaE_BufferTypes_ForMemoryManager::ENUM_MultithreadedSimpleMemoryBuffer);

	point1 = manager.GetMemory(key1, 10);
	point1 = manager.GetMemory(key1, 100);
	point1 = manager.GetMemory(key1, 1010);

	manager.Clear_Buffer(key1);

	manager.DeleteMemoryBuffer(&key4);

	manager.DeleteMemoryBuffer(&key3);
	manager.DeleteMemoryBuffer(&key2);
	manager.Free_Manager();

	manager.DeleteMemoryBuffer(&key1);

	void* endpoint = nullptr;
}
*/


// class for testing
class TestClass 
{
<<<<<<< HEAD
	//OPTada_C_CiclicBuffer_Memory asd(1); 
	//OPTada_S_CiclicBufferElement *ff;

	/*int re = sizeof(OPTada_S_CyclicBufferElement_Memory);
	OPTada_C_CiclicBuffer_Memory asd(50003);

	OPTada_S_CyclicBufferElement_Memory* one = nullptr;
	OPTada_S_CyclicBufferElement_Memory* two = nullptr;
	OPTada_S_CyclicBufferElement_Memory* tree = nullptr;
	OPTada_S_CyclicBufferElement_Memory* fore = nullptr;






	CRITICAL_SECTION ThreadSynchronization; // критическая секция для синхронизации потоков

	InitializeCriticalSection(&ThreadSynchronization); // инициализация критической секции

	DeleteCriticalSection(&ThreadSynchronization); // удаление критической секции

	EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

	LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку

=======
public:
>>>>>>> dev

	int  i = 0;
	char a = 's';

	TestClass() 
	{
		i = 3;
		a = 'd';
	}

	~TestClass()
	{
		i = 2;
		a = 'x';
	}
};


void timer()
{
	clock_t start, end;
	// create and init code here

	OPTada_MemoryManager manager;
	manager.Init_Mamager(3);
	OPTadaS_Key_MemoryManager* key = manager.CreateNewMemoryBuffer(10, 100000000, 10000010, 8, OPTadaE_BufferTypes_ForMemoryManager::ENUM_SimpleMemoryBuffer);
	cout << endl << "GetAllModulesLockedMemory memory managaer (in bytes) = " << manager.Get_AllModulesLockedMemory() <<  endl;

	TestClass* link = nullptr;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	cout << endl << "Classic new \ delete:" << endl;
	start = clock();
	// The code whose runtime is to be measured

	for (int i = 0; i < 10000000; i++) {
		link = new TestClass();
		delete (link);
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	end = clock();
	printf("The above code block was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC));
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	cout << endl << "Memory manager" << endl;
	start = clock();
	// The code whose runtime is to be measured

	for (int i = 0; i < 10000000; i++) {
		link = (TestClass*)manager.GetMemory(key, sizeof(TestClass));
		if (link) {
			link->TestClass::TestClass();
			link->~TestClass();
			manager.ReturnMemory(key, link);
		}
		else {
			cout << endl << "We Can't take memory !!!" << endl;
			break;
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	end = clock();
	printf("The above code block was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC));
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	manager.Free_Manager();

	int s;
	cin >> s;
}



void main()
{
	cout << " ---> MAIN enter point <---" << endl << endl;

	// you can test manager here.
	//timer();

	cout << endl << "- - - - - - - - END - - - - - - - - -" << endl;
}
