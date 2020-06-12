#pragma once

#include "OPTada_Memory_C_CyclicMemoryElemsBuffer.h"

// класс - шаблон для различных буфферов +
class OPTada_Memory_C_TemplateMemoryBuffer
{
protected:

	char * Buffer = nullptr; // указатель на буффер памяти
	size_t Buffer_Length = 0; // общий размер памяти доступной в етом буффере (в байтах)
	size_t Locked = 0; // колличество памяти занятой в етом буффере (в байтах)

public:

	virtual bool Clear_Buffer() = 0; // virtual Метод очищает всю память в буффере для дальнейшей работы с ней заново (! память помечается свободной и не вызывает деструкторы)

	virtual void * GetMemory(size_t New_Length_) = 0; // virtual Метод попытается найти ближайший кусок памяти и вернуть его (выделить для использования)
	virtual bool ReturnMemory(void * Link_) = 0; // virtual Метод пометит указанный участок памяти как "свободная"

	virtual bool Get_TestBuffer() = 0; // virtual Метод проводит тест буффера на наличие ошибок при создании
	virtual size_t Get_LockedMemory() = 0; // virtual Метод возвращает колличество занятой памяти в буффере (в байтах)
	virtual size_t Get_AllModulesLockedMemory() = 0; // virtual Метод возвращает колличество занятой памяти вообще (+ дополнительные буфферы) (в байтах)
	virtual size_t Get_Size() = 0; // virtual Метод возвращает размер буффера (в байтах)
};