#pragma once

#include "MemoryManager_Settings.h"

// структура - элемент для создания связaных списков (56 Б)
struct OPTada_Memory_S_CyclicMemoryElemsBufferElement
{
public:
	size_t Size = 0;    // длина буфера в байтах (если 0 - буффер свободен)
	char * Link = NULL; // указатель на память
	bool free = true;   // переменная обазначающая тип памяти (свободна?\нет?)

	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Next_el = NULL;     // указатель на следующий элемент
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Previous_el = NULL; // указатель на предидущий элемент 

	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Next_link = NULL;     // указатель на следующий элемент (во втором буффере)
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Previous_link = NULL; // указатель на предидущий элемент (во втором буффере)

	// Конструктор OPTada_S_CiclicBufferElement
	OPTada_Memory_S_CyclicMemoryElemsBufferElement();
};



// проводить тест после создания для контроля ошибок
// класс - цыклический буффер хранящий ячейки OPTada_Memory_S_CyclicMemoryElemsBufferElement (для создания связаных списков буфферов менеджера памяти)
class OPTada_Memory_C_CyclicMemoryElemsBuffer
{
private:

	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Buffer = NULL;      // буфер элементов
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Buffer_last = NULL; // указатель на начало последнего элемента в буффере
	size_t Length = 0; // Общеее колличество ячеек
	size_t Locked = 0; // колличество захваченых ячеек
	size_t Index  = 0; // указатель на текущий элемент в буффере (на ближайшей свободной, иначе 0)	

public:

	// Конструктор OPTada_Memory_C_CyclicMemoryElemsBuffer
	// [in] size_t Size_ // Размер создаваемого буффера - колличество создаваемых ячеек
	OPTada_Memory_C_CyclicMemoryElemsBuffer(size_t Size_);

	// Деструктор OPTada_Memory_C_CyclicMemoryElemsBuffer
	~OPTada_Memory_C_CyclicMemoryElemsBuffer();


	// Запрос на свободный элемент
	// return = ссылка на элемент | NULL - выдать элемент невозможно (ошибка)
	OPTada_Memory_S_CyclicMemoryElemsBufferElement * Get_Element(); 

	// Возвращает элемент в кучу (освобождает его) - !! в ячейке остается мусор !!
	// [in] OPTada_Memory_S_CyclicMemoryElemsBufferElement * Elem_ // Указатель на элемент
	// return = true - успешно | false - ошибка
	bool Return_Element(OPTada_Memory_S_CyclicMemoryElemsBufferElement * Elem_);

	// Возвращает колличество памяти потребляемое етим классом (+ захваченая память)
	// return = колличество захваченой памяти етим классом (включая захваченую) (в байтах)
	size_t Get_LockedAllMemory();
};