
#include "MamoryManager_x64\OPTada_MemoryManager.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;


OPTada_MemoryManager MemoryManager; // ������������������ �������� (��������� � ����������)


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






	CRITICAL_SECTION ThreadSynchronization; // ����������� ������ ��� ������������� �������

	InitializeCriticalSection(&ThreadSynchronization); // ������������� ����������� ������

	DeleteCriticalSection(&ThreadSynchronization); // �������� ����������� ������

	EnterCriticalSection(&ThreadSynchronization); // ������ �� ������ (��� �������������)

	LeaveCriticalSection(&ThreadSynchronization); // ��������� ������ ���������� ������




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

	asd.Return_Element(tree);*/

	//OPTada_Memory_C_SimpleMemoryBuffer * buf = (OPTada_Memory_C_SimpleMemoryBuffer *)malloc(sizeof(OPTada_Memory_C_SimpleMemoryBuffer));
	//buf->OPTada_Memory_C_SimpleMemoryBuffer::OPTada_Memory_C_SimpleMemoryBuffer(10000000, 10000000);

	//OPTada_C_CyclicBuffer_Memory asd(100000);

	//rrq * ff;

	//CRITICAL_SECTION cs; // ���� ������
	//InitializeCriticalSection(&cs);

	//mutex mut; // ������

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
	//for (int i = 0; i < 10000000; i++)
	//{
	//	/*ff = new OPTada_S_CiclicBufferElement;
	//	delete (ff);*/
	//}
	/* ���, ����� ���������� �������� ����� �������� */
	/*  */

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
	r = (B *) g;*/

	//// �������� ������
	//LOLO * keke = (LOLO *)malloc(sizeof(LOLO) * 1); // ������ ������
	////new(keke) LOLO; // ����� ������������ !!! 
	//keke->LOLO::LOLO(); // ���� ����� ������������ (���������)
	//// ������ � ��������

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





	//class1 * keke = ��������� ������
	//keke->class1::class1(); // ����� ������������
	////
	//keke->~LOLO(); // ����� ����������� (��������)
	//free(keke); // ������������ ������
	//// ����������



	//OPTada_C_CiclicBuffer_Memory Test(5);
	//OPTada_S_CiclicBufferElement* el = Test.Get_Element();
	//el->Link = malloc(sizeof(rt));
	//rt* obj = (rt*)el->Link;
	//*(dt*)(el->Link) = dt(); // ���������� ����� ������������
	//((dt*)(el->Link))->~dt(); // ���������� ����� �����������
	//free(el->Link);

}


// ������
// ������ � ������ (������)
// ��������� � ������ (���������� \ ������)

//->Link = T::~T();   tamplate<class T> !!! ��� �����!