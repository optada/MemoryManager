#pragma once

#include "OPTada_C_TemplateMemoryBuffer.h"


// Класс - стандартный буффер памяти 
// Желательно использовать для одноразовой загрузки данных или для данных на полную перезапись 
// При запроссе пытается найти ячейку с достаточным объемом, отделяет от нее необходимый кусок и выдает пользователю
class OPTada_C_SimpleMemoryBuffer : public OPTada_C_TemplateMemoryBuffer
{
protected:

	OPTada_S_CyclicMemoryElemsBufferElement * FirstCell_Buffer = NULL;   // Указатель на первую ячейку
	OPTada_S_CyclicMemoryElemsBufferElement * FreeCells_Buffer = NULL;   // Буфеер свободных ячеек
	OPTada_S_CyclicMemoryElemsBufferElement * LockedCells_Buffer = NULL; // Буффер заблокированных ячеек
	OPTada_C_CyclicMemoryElemsBuffer * Elem_Buffer = NULL; // Буффер с ячейками - элементами (для выдачи и получения)

	size_t Cell_Size = 0; // коэфициент деления инфрормации (для уменьшения фрагментации) (байт)

	// Метод ищет свободную память, если найдена - выполняет всю работу и выделяет
	void * TakeMemoryMethod(size_t New_size_);

public:

	// Конструктор OPTada_Memory_C_SimpleMemoryBuffer
	// Память будет запрошена у ОС, дополнительный буффер будет создан
	// [in] size_t Size_ // Размер создаваемого буффера (байт)
	// [in] size_t Elem_Buffer_Size_ // Размер буффера элементов (дополнительный буффер) (колличество элементов)
	// [in] size_t Cell_Size_ // Размер ячейки (коэфициента деления) для уменьшения фрагментации
	OPTada_C_SimpleMemoryBuffer(size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_);

	//Деструктор OPTada_Memory_C_SimpleMemoryBuffer
	~OPTada_C_SimpleMemoryBuffer();


	// !!! ВНИМАНИЕ !!! память помечается свободной и не вызывает деструктор(ы)
	// Метод очищает всю память в буффере для дальнейшей работы с ней заново
	// return = true - удачно | false - ошибка
	bool Clear_Buffer();


	// !!! ВНИМАНИЕ !!! не инициализируется, только выделяется память.
	// Метод попытается найти ближайший кусок памяти и вернуть его (выделить для использования) не массив
	// [in] size_t New_Length_ // необходимая длина куска памяти (в байтах)
	// return = если удачно вернет указатель на память | NULL - если не удалось
	void * GetMemory(size_t New_Length_);

	// !!! ВНИМАНИЕ !!! деструктор не вызывается, память помечается как "свободная"
	// Метод пометит указанный участок памяти как "свободная"
	// [in] void * Link_ // Указатель на память которую нужно освободить
	// return = true - удачно | false - ошибка
	bool ReturnMemory(void * Link_);


	// Метод проводит тест буффера на наличие ошибок при создании
	// return = true - буффер удачен | false - ошибка при создании буффера
	bool Get_TestBuffer();

	// Метод возвращает колличество занятой памяти в буффере (в байтах)
	// return = колличество занятой памяти в етом буффере
	size_t Get_LockedMemory();

	// Метод возвращает колличество занятой памяти (+ дополнительные буфферы) (в байтах)
	// return = число - колличество памяти занятой у системы етим классим и его компонентами
	size_t Get_AllModulesLockedMemory();

	// Метод возвращает размер буффера (в байтах)
	// return = число - размер буффера (в байтах) | 0 - буффер не создан/ошибка
	size_t Get_Size();
};