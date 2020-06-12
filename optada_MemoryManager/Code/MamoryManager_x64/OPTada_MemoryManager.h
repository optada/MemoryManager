#pragma once

#include "OPTada_Memory_C_TemplateMemoryBuffer.h"
#include "OPTada_Memory_C_SimpleMemoryBuffer.h"
#include "OPTada_Memory_C_MultithreadedSimpleMemoryBuffer.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// ВНИМАНИЕ!!! ПРОВЕРКИ НА ОШИБКИ ЕСТЬ, НО НЕТУ ЛОГИКИ ДЕЙСТВИЙ ПРИ ОШИБКАХ В КОНСТРУКТОРАХ!!! //
// НЕОБХОДИМО ПРОВЕРИТЬ БУФФЕР ПОСЛЕ СОЗДАНИЯ												   //
//		СОЗДАТЬ БУФФЕР																		   //
//		ЗАПРОСИТЬ МИНИМАЛЬНУЮ ЯЧЕЙКУ ПАМЯТИ													   //
// ЕСЛИ ПРИ ЗАПРОСЕ ПАМЯТИ ВЫ ПОЛУЧИЛИ NULL - СКОРЕЕ ВСЕГО ОШИБКА ПРИ СОЗДАНИИ БУФФЕРА!!!!	   //
// Добавлена проверка при создании буффера на ошибки                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////


// структура для доступа к буфферу памяти +
struct OPTada_Memory_S_MemoryManedger
{
	int nomer = -1; // указатель на определенный буффер в менеджере памяти
	int type = -1; // если -1 - не инициализирован, если 1 - < то тип памяти (зачем? я - хз, для деструктора наверное)
};

// структура для менеджера памяти +
struct OPTada_Memory_S_MemoryManagerElement_Memory
{
public:
	OPTada_Memory_C_TemplateMemoryBuffer * Buffer = NULL; // ссылка на общий класс (элемент - буффер)
	bool Element_locked = false; // false - ячейка свободна | true - заблокирована (занята)
	int BufferType = 0; // тип буффера
};

sss // добавить контроль многопоточности при создании новых буферов

// менеджер памяти
// !! требует инициализации !!
class OPTada_MemoryManager
{
private:

	OPTada_Memory_S_MemoryManagerElement_Memory* Elem_buffer = NULL; // Буффер ячеек - элементов
	int Size_buff = 0; // Колличиство ячеек в буффере (если 0 - буффер не инициализирован)

public:

	OPTada_Memory_S_MemoryManedger New_Buffer; // указатель на нулевой буффер (для общего использования)

	// инициализирует менеджер памяти (если он был инициализирован - возвратит ошибку)
	// [in] int Size_buff_ // Колличество буфферов которые будут созданы (+1 нулевой new)
	// return = true - успешно | false - ошибка
	bool Init_Mamager(int Size_buff_);

	// освобождает всю захваченую память в менеджере (удаляет все буфферы и освобождает память)
	// return = true - успешно | false - ошибка
	bool Free_Manager();


	// Метод создает новый буффер по указанной ячейке памяти (кроме 0) и возвращает Buffer_MemoryManedger *
	// [in] int type_ // тип создаваемого буффера
	//		1 - стандартный
	//		2 - стандартный с поддержкой многопоточности
	// [in] size_t Size_ // Объем памяти в создаваемом буффере
	// [in] size_t Elem_Buffer_Size_ // Колличество ячеек-запросов в создаваемом буффере
	// return = при удаче вернет указатель на структуру для обращения к созданному буферу | NULL - при ошибке
	OPTada_Memory_S_MemoryManedger * CreateNewBuffer(int type_, size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_);


	// Метод выделяет память запрашиваемого размера (цельный кусок без дополнительной информации)
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер
	// [in] size_t Size_ // запрашиваемый объем памяти
	// return = указатель на память в случае успеха | NULL - при ошибке
	void * GetMemory(OPTada_Memory_S_MemoryManedger * Link_Buffer_, size_t Size_);


	// !!! ВНИМАНИЕ !!! деструктор не вызывается, память помечается как "свободная"
	// Метод пометит указанный участок памяти как "свободная"
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер
	// [in] void * Link_ // Указатель на память которую нужно освободить
	// return = true - удачно | false - ошибка
	bool ReturnMemory(OPTada_Memory_S_MemoryManedger * Link_Buffer_, void * Link_);


	// !!! ВНИМАНИЕ !!! память помечается свободной и не вызывает деструктор(ы)
	// Метод очищает всю память в буффере для дальнейшей работы с ней заново
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер
	// return = true - удачно | false - ошибка
	bool Clear_Buffer(OPTada_Memory_S_MemoryManedger * Link_Buffer_);


	// Метод возвращает колличество занятой памяти в буффере (в байтах)
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер, если NULL - со всех буфферов
	// return = колличество занятой памяти в етом буффере
	size_t Get_LockedMemory(OPTada_Memory_S_MemoryManedger * Link_Buffer_);

	// Метод возвращает колличество занятой памяти вообще (+ дополнительные буфферы и размер етого класса) (в байтах)
	// return = число - колличество памяти занятой у системы етим классим и его компонентами
	size_t Get_AllModulesLockedMemory();

	// Метод возвращает размер буффера (в байтах)
	// [in] OPTada_Memory_S_MemoryManedger * Link_Buffer_ // Структура для указания на необходимый буффер
	// return = число - размер буффера (в байтах) | 0 - буффер не создан/ошибка
	size_t Get_Size(OPTada_Memory_S_MemoryManedger * Link_Buffer_);

	 
	//// Конструктор OPTada_GSC_MemoryManager
	//OPTada_GSC_MemoryManager();
	//// Деструктор OPTada_GSC_MemoryManager
	//~OPTada_GSC_MemoryManager();
};


//void *operator new(size_t size)
//{
//	return NULL;
//}

//void *operator new(size_t size, OPTada_Memory_S_MemoryManedger * Link_Buffer_)
//{
//	return NULL;
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
//// return = если удачно вернет указатель на память | NULL - если не удалось
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
//			return NULL;
//		}
//	}
//	return NULL;
//}



