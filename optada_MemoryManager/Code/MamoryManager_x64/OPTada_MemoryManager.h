// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTada_MemoryManager_Settings.h"

#include "SupportBuffers\OPTadaS_TemplateMemoryBuffer_Element.h"

#include "SupportBuffers\OPTadaC_TemplateOfMemoryBuffer.h"
#include "SupportBuffers\OPTadaC_SimpleMemoryBuffer.h"
#include "SupportBuffers\OPTadaC_MultithreadedSimpleMemoryBuffer.h"


/////////////////////////////////////////////////////////////////////////////////////////////////
// ВНИМАНИЕ!!! ПРОВЕРКИ НА ОШИБКИ ЕСТЬ, НО НЕТУ ЛОГИКИ ДЕЙСТВИЙ ПРИ ОШИБКАХ В КОНСТРУКТОРАХ!!! //
// НЕОБХОДИМО ПРОВЕРИТЬ БУФФЕР ПОСЛЕ СОЗДАНИЯ												   //
//		СОЗДАТЬ БУФФЕР																		   //
//		ЗАПРОСИТЬ МИНИМАЛЬНУЮ ЯЧЕЙКУ ПАМЯТИ													   //
// ЕСЛИ ПРИ ЗАПРОСЕ ПАМЯТИ ВЫ ПОЛУЧИЛИ nullptr - СКОРЕЕ ВСЕГО ОШИБКА ПРИ СОЗДАНИИ БУФФЕРА!!!!  //
// Добавлена проверка при создании буффера на ошибки                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////


// структура для доступа к буфферу памяти +
struct OPTadaS_MemoryManedger
{
	int nomer = -1; // указатель на определенный буффер в менеджере памяти
	int type = -1;  // если -1 - не инициализирован, если 1 - < то тип памяти (зачем? я - хз, для деструктора наверное)
};


//TODO // добавить контроль многопоточности при создании новых буферов






// менеджер памяти
// !! требует инициализации !!
class OPTada_MemoryManager
{
private:

	OPTadaS_TemplateMemoryBuffer_Element* buffer_OfMemoryBuffers = nullptr; // Буффер ячеек - элементов
	int size_buffer_OfMemoryBuffers                              = 0;       // Колличиство ячеек в буффере (если 0 - буффер не инициализирован)

	CRITICAL_SECTION threadSynchronization; // critical section for thread synchronization

public:

	// инициализирует менеджер памяти (если он был инициализирован - возвратит ошибку)
	// [in] int Size_buff_ // Колличество буфферов которые будут созданы (+1 default new)
	// return = true - успешно | false - ошибка
	bool Init_Mamager(int Size_buff_);

	// освобождает всю захваченую память в менеджере (удаляет все буфферы и освобождает память)
	// return = true - успешно | false - ошибка
	bool Free_Manager();


	// Метод создает новый буффер по указанной ячейке памяти (кроме 0) и возвращает Buffer_MemoryManedger *
	// [in] int type_ // тип создаваемого буффера
	// [in] size_t Size_ // Объем памяти в создаваемом буффере
	// [in] size_t Elem_Buffer_Size_ // Колличество ячеек-запросов в создаваемом буффере
	// return = при удаче вернет указатель на структуру для обращения к созданному буферу | nullptr - при ошибке
	OPTadaS_MemoryManedger* CreateNewMemoryBuffer(int type_, size_t memoryLength_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_);

	// метод очищает и полностью удаляет переданный буффер памяти
	// [in] OPTadaS_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер
	// return = true - успешно | false - ошибка
	bool DeleteMemoryBuffer(OPTadaS_MemoryManedger* link_Buffer_);


	// !- memory is marked free and does not call destructors -!
	// The method clears all the memory in the buffer for further work with it again
	// [in] OPTadaS_MemoryManedger* link_Buffer_ // Structure to indicate the required buffer
	// return = true - successful | false - error
	bool Clear_Buffer(OPTadaS_MemoryManedger* link_Buffer_);

	// Метод выделяет память запрашиваемого размера (цельный кусок без дополнительной информации)
	// [in] OPTadaS_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер
	// [in] size_t Size_ // запрашиваемый объем памяти
	// return = указатель на память в случае успеха | nullptr - при ошибке
	void* GetMemory(OPTadaS_MemoryManedger* Link_Buffer_, size_t Size_);

	// !!! ВНИМАНИЕ !!! деструктор не вызывается, память помечается как "свободная"
	// Метод пометит указанный участок памяти как "свободная"
	// [in] OPTadaS_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер
	// [in] void * Link_ // Указатель на память которую нужно освободить
	// return = true - удачно | false - ошибка
	bool ReturnMemory(OPTadaS_MemoryManedger* Link_Buffer_, void * Link_);

	// The method returns the count of locked memory in the buffer (in bytes)
	// [in] OPTadaS_MemoryManedger* link_Buffer_ // Structure to indicate the required buffer, if nullptr - from all buffers
	// return = count of memory used (locked) in this buffer
	size_t Get_LockedMemory(OPTadaS_MemoryManedger* link_Buffer_);

	// The method returns the size of the memory buffer (in bytes)
	// [in] OPTadaS_MemoryManedger* link_Buffer_ // Structure to indicate the required buffer, if nullptr - from all buffers
	// return = number - buffer size(in bytes) | 0 - buffer not created / error
	size_t Get_BufferMemorySize(OPTadaS_MemoryManedger* link_Buffer_);

	// The method returns the amount of occupied memory of self (+ additional buffers) (in bytes)
	// return = number - the amount of memory used by the system and its components (in bytes)
	size_t Get_AllModulesLockedMemory();
};


//void *operator new(size_t size)
//{
//	return nullptr;
//}

//void *operator new(size_t size, OPTada_Memory_S_MemoryManedger * Link_Buffer_)
//{
//	return nullptr;
//}

//// !!! ВНИМАНИЕ !!! деструктор не вызывается, память помечается как "свободная"
//// Метод пометит указанный участок памяти как "свободная" (для памяти на массив)
//// будет использован сдвиг указателя
//// [in] void * Link_ // Указатель на память которую нужно освободить
//// return = true - удачно | false - ошибка 
//bool ReturnMemoryMass(void * Link_)
//{
//	size_t * del_link = (size_t *)Link_;
//	return ReturnMemory((--del_link));
//}



//// !!! ВНИМАНИЕ !!! не инициализируется, только выделяется память.
//// Метод попытается найти ближайший кусок памяти и вернуть его (выделить для использования) для массива
//// [in] size_t New_Length_ // необходимая длина куска памяти (в байтах)
//// [in] size_t Col_mass_ // колличество элементов в данном участке памяти (для вызова деструкторов)
//// return = если удачно вернет указатель на память | nullptr - если не удалось
//void * GetMemoryMass(size_t New_Length_, size_t Col_mass_)
//{
//	size_t * link;
//	if (Col_mass_ > 0)
//	{ // ето массив
//		if ((link = (size_t *)TakeMemoryMethod(sizeof(size_t) + New_Length_))) // выделили еще 8 байт для хранения размера массива
//		{
//			*link = Col_mass_;
//			return (++link);
//		}
//		else
//		{
//			return nullptr;
//		}
//	}
//	return nullptr;
//}



