
// #include "MamoryManager_x64\OPTada_MemoryManager.h"


// tested
//#include "MamoryManager_x64\OPTadaC_MemoryCells_StaticCyclicBuffer.h"
//#include "MamoryManager_x64\OPTadaC_SimpleMemoryBuffer.h"
//#include "MamoryManager_x64\OPTadaC_MultithreadedSimpleMemoryBuffer.h"


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;


// test for OPTadaC_MemoryCells_StaticCyclicBuffer | OK
/*
void testFor_OPTadaC_MemoryCells_StaticCyclicBuffer()
{
	bool del = false;
	OPTadaC_MemoryCells_StaticCyclicBuffer Buffer = OPTadaC_MemoryCells_StaticCyclicBuffer(5, del);

	cout << endl << del;
	cout << endl << Buffer.Get_AllCapturedMemory();
	
	void* element1 = NULL;
	void* element2 = NULL;
	void* element3 = NULL;
	void* element4 = NULL;
	void* element5 = NULL;
	void* element6 = NULL;

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

	void* pok1 = NULL;
	void* pok2 = NULL;
	void* pok3 = NULL;
	void* pok4 = NULL;
	void* pok5 = NULL;
	void* pok6 = NULL;

	// incorrect or specifick input
	pok1 = Buffer.GetMemory(-1); // ! unsigned parameter
	pok1 = Buffer.GetMemory(1000000); // fragmetation +0 (2b - fragmet cell)
	pok1 = Buffer.GetMemory(1000001); // fragmetation +0 (2b - fragmet cell)
	
	// incorrect reff
	int a;
	pok1 = &a;
	Buffer.ReturnMemory(pok1);
	pok1 = NULL;
	Buffer.ReturnMemory(pok1);

	// try get memory errors
	pok1 = Buffer.GetMemory(8);
	// low memory
	pok2 = Buffer.GetMemory(3); // we have 2 now
	// have no enaght memory
	pok3 = Buffer.GetMemory(11); // we have 10 max

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = NULL;
	pok2 = NULL;
	pok3 = NULL;

	// fragmentaion mamory error +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00 
	// now we have 8 memory (2 * 4 cells)
	pok4 = Buffer.GetMemory(8); // error (NULL)

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = NULL;
	pok2 = NULL;
	pok3 = NULL;
	pok4 = NULL;

	// marege free memory +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00 
	Buffer.ReturnMemory(pok2); // 00 00-00-00 00

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = NULL;
	pok2 = NULL;
	pok3 = NULL;
	pok4 = NULL;

}
*/

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

	void* pok1 = NULL;
	void* pok2 = NULL;
	void* pok3 = NULL;
	void* pok4 = NULL;
	void* pok5 = NULL;
	void* pok6 = NULL;

	// incorrect or specifick input
	pok1 = Buffer.GetMemory(-1); // ! unsigned parameter
	pok1 = Buffer.GetMemory(1000000); // fragmetation +0 (2b - fragmet cell)
	pok1 = Buffer.GetMemory(1000001); // fragmetation +0 (2b - fragmet cell)

	// incorrect reff
	int a;
	pok1 = &a;
	Buffer.ReturnMemory(pok1);
	pok1 = NULL;
	Buffer.ReturnMemory(pok1);

	// try get memory errors
	pok1 = Buffer.GetMemory(8);
	// low memory
	pok2 = Buffer.GetMemory(3); // we have 2 now
	// have no enaght memory
	pok3 = Buffer.GetMemory(11); // we have 10 max

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = NULL;
	pok2 = NULL;
	pok3 = NULL;

	// fragmentaion mamory error +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00
	// now we have 8 memory (2 * 4 cells)
	pok4 = Buffer.GetMemory(8); // error (NULL)

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = NULL;
	pok2 = NULL;
	pok3 = NULL;
	pok4 = NULL;

	// marege free memory +
	pok1 = Buffer.GetMemory(4); // XX XX 00 00 00
	pok2 = Buffer.GetMemory(2); // XX XX XX 00 00
	pok3 = Buffer.GetMemory(4); // XX XX XX XX XX
	Buffer.ReturnMemory(pok1); // 00 00 XX XX XX
	Buffer.ReturnMemory(pok3); // 00 00 XX 00 00
	Buffer.ReturnMemory(pok2); // 00 00-00-00 00

	// refresh buffer
	Buffer.Clear_Buffer();
	pok1 = NULL;
	pok2 = NULL;
	pok3 = NULL;
	pok4 = NULL;

}
*/


void main()
{
	cout << "MAIN enter point" << endl;


	cout << endl << "- - - - - - - - END - - - - - - - - -" << endl;
}



















/*

OPTada_MemoryManager MemoryManager; // инициализированный менеджер (перенести в глобальные)


int timer()
{
	//OPTada_C_CiclicBuffer_Memory asd(1); 
	//OPTada_S_CiclicBufferElement *ff;

	/*int re = sizeof(OPTada_S_CyclicBufferElement_Memory);
	OPTada_C_CiclicBuffer_Memory asd(50003);

	OPTada_S_CyclicBufferElement_Memory* one = NULL;
	OPTada_S_CyclicBufferElement_Memory* two = NULL;
	OPTada_S_CyclicBufferElement_Memory* tree = NULL;
	OPTada_S_CyclicBufferElement_Memory* fore = NULL;



	




	CRITICAL_SECTION ThreadSynchronization; // критическая секция для синхронизации потоков

	InitializeCriticalSection(&ThreadSynchronization); // инициализация критической секции

	DeleteCriticalSection(&ThreadSynchronization); // удаление критической секции

	EnterCriticalSection(&ThreadSynchronization); // запрет на доступ (для синхронизации)

	LeaveCriticalSection(&ThreadSynchronization); // разрешили доступ следующему потоку




	one = asd.Get_Element();
	one->Length = 4;
	one->Size = 1;
	two = asd.Get_Element();
	two->Length = 7;
	two->Size = 2;
	for (int i = 0; i < 50000; i++)
	{
		fore = asd.Get_Element();
		fore->Size = 4;
	}
	tree = asd.Get_Element();
	tree->Length = 8;
	tree->Size = 3;
	
	asd.Return_Element(one);
	one = asd.Get_Element();
	one->Length = 4;
	one->Size = 1;

	asd.Return_Element(tree);

	//OPTada_Memory_C_SimpleMemoryBuffer * buf = (OPTada_Memory_C_SimpleMemoryBuffer *)malloc(sizeof(OPTada_Memory_C_SimpleMemoryBuffer));
	//buf->OPTada_Memory_C_SimpleMemoryBuffer::OPTada_Memory_C_SimpleMemoryBuffer(10000000, 10000000);

	//OPTada_C_CyclicBuffer_Memory asd(100000);

	//rrq * ff;

	//CRITICAL_SECTION cs; // крит секция
	//InitializeCriticalSection(&cs);

	//mutex mut; // мютекс

	clock_t start, end;

	start = clock();

	//for (int i = 0; i < 10000000; i++)
	//{
	//	ff = (rrq *)buf->GetMemory(1);
	//	ff->rrq::rrq();
	//}

	/*tree = asd.Get_Element();
	tree->Length = 8;
	tree->Size = 3;*/
/*
	//for (int i = 0; i < 10000000; i++)
	//{
	//	/*ff = new OPTada_S_CiclicBufferElement;
	//	delete (ff);
	//}
	// Код, время выполнения которого нужно измерить 
	

*/
/*
	end = clock();

	printf("The above code block was executed in %.4f second(s)\n", ((double)end - start) / ((double)CLOCKS_PER_SEC));


	int s;
	cin >> s;
	return 0;
}



void main()
{

	size_t asdddds = sizeof(OPTada_Memory_S_CyclicMemoryElemsBufferElement);

	MemoryManager.Init_Mamager(5);
	MemoryManager.Free_Manager();
	


	/*OPTada_Memory_C_MultithreadedSimpleMemoryBuffer * a = new OPTada_Memory_C_MultithreadedSimpleMemoryBuffer(120, 120);
	delete(a);*/
/*
	//OPTada_Memory_C_SimpleMemoryBuffer * buf = (OPTada_Memory_C_SimpleMemoryBuffer *)malloc(sizeof(OPTada_Memory_C_SimpleMemoryBuffer));
	//buf->OPTada_Memory_C_SimpleMemoryBuffer::OPTada_Memory_C_SimpleMe
	//memoryBuffer(31, 10);
	//for (int i = 0; i < 31; i++)
	//{
	//	buf->FirstCell_Buffer->Link[i] = (i + 1);
	//}
	//for (int i = 0; i < 31; i++)
	//{
	//	cout << " " << (int)(buf->FirstCell_Buffer->Link[i]);
	//}
	//
	//char * dd[11];

	//dd[1] = (char *)buf->GetMemory(2);
	//dd[2] = (char *)buf->GetMemory(4);
	//dd[3] = (char *)buf->GetMemory(6);
	//dd[4] = (char *)buf->GetMemory(2);
	//dd[5] = (char *)buf->GetMemory(15);
	//dd[6] = (char *)buf->GetMemory(0);
	//dd[7] = (char *)buf->GetMemory(2);
	//dd[8] = (char *)buf->GetMemory(1);
	//dd[9] = (char *)buf->GetMemory(1);

	//buf->ReturnMemory(dd[3]);
	//buf->ReturnMemory(dd[5]);
	//buf->ReturnMemory(dd[4]);

	//dd[3] = (char *)buf->GetMemory(6);
	//dd[4] = (char *)buf->GetMemory(2);
	//dd[5] = (char *)buf->GetMemory(15);

	//buf->ReturnMemory(dd[1]);

	//cout << endl << "1 " << (int)(*dd[1]);
	//cout << endl << "2 " << (int)(*dd[2]);
	//cout << endl << "3 " << (int)(*dd[3]);
	//cout << endl << "4 " << (int)(*dd[4]);
	//cout << endl << "5 " << (int)(*dd[5]);
	//cout << endl << "7 " << (int)(*dd[7]);

	//cout << endl << buf->Get_LockedMemory();

	//cout << endl << "------2------" << endl;

	//buf->ReturnMemory(dd[2]);
	//cout << endl << buf->Get_LockedMemory();

	//cout << endl << "------3------" << endl;

	//// buf->ReturnMemory(dd[5]);
	//cout << endl << buf->Get_LockedMemory();

	//cout << endl << "------4------" << endl;

	//buf->ReturnMemory(dd[3]);
	//cout << endl << buf->Get_LockedMemory();

	//cout << endl << "------5------" << endl;

	//buf->ReturnMemory(dd[4]);
	//cout << endl << buf->Get_LockedMemory();

	//buf->ReturnMemory(dd[1]);

	//cout << endl << "1 " << (int)(*dd[1]);


	//cout << endl << buf->Get_LockedMemory();


	//OPTada_Memory_C_TemplateMemoryBuffer * bu;
	//OPTada_Memory_C_MultithreadedSimpleMemoryBuffer * gtt;
	//gtt = (OPTada_Memory_C_MultithreadedSimpleMemoryBuffer*)malloc(sizeof(OPTada_Memory_C_MultithreadedSimpleMemoryBuffer));
	//gtt->OPTada_Memory_C_MultithreadedSimpleMemoryBuffer::OPTada_Memory_C_MultithreadedSimpleMemoryBuffer(100, 50, 1);
	//bu = gtt;

	//bu->Get_TestBuffer();
	//void * link1 = bu->GetMemoryMass(15, 5);
	//void * link2 = bu->GetMemory(50);
	//void * link3 = bu->GetMemory(500);

	//bu->ReturnMemory(link2);
	//bu->ReturnMemoryMass(link1);

	timer();
	
	/*OPTada_C_TemplateBuffer * g = NULL;
	g = (OPTada_C_TemplateBuffer *) new B();
	B * r;
	r = (B *) g;

	//// создание класса
	//LOLO * keke = (LOLO *)malloc(sizeof(LOLO) * 1); // запрос памяти
	////new(keke) LOLO; // вызов конструктора !!! 
	//keke->LOLO::LOLO(); // тоже вызов конструктора (запутался)
	//// работа с объектом
*/
/*int re = sizeof(OPTada_S_CiclicBufferElement);
	OPTada_C_CiclicBuffer_Memory asd(100000);

	OPTada_S_CiclicBufferElement* one = NULL;
	OPTada_S_CiclicBufferElement* two = NULL;
	OPTada_S_CiclicBufferElement* tree = NULL;
	OPTada_S_CiclicBufferElement* fore = NULL;

	
	one = asd.Get_Element();
	one->Length = 4;
	one->Size = 1;
	two = asd.Get_Element();
	two->Length = 7;
	two->Size = 2;
	tree = asd.Get_Element();
	tree->Length = 8;
	tree->Size = 3;
	for (int i = 0; i < 50000; i++)
	{
		fore = asd.Get_Element();
		fore->Size = 4;
	}
	asd.Return_Element(two);*/
//class1 * keke = получение памяти
	//keke->class1::class1(); // вызов конструктора
	////
	//keke->~LOLO(); // вызов деструктора (удаление)
	//free(keke); // освобождение памяти
	//// завершение
	//OPTada_C_CiclicBuffer_Memory Test(5);
	//OPTada_S_CiclicBufferElement* el = Test.Get_Element();
	//el->Link = malloc(sizeof(rt));
	//rt* obj = (rt*)el->Link;
	//*(dt*)(el->Link) = dt(); // корректный вызов конструктора
	//((dt*)(el->Link))->~dt(); // корректный вызов деструктора
	//free(el->Link);
/*
}


// потоки
// доступ к данным (потоки)
// адресация в памяти (сохранение \ размер)

//->Link = T::~T();   tamplate<class T> !!! учи идиот!


*/