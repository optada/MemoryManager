#include "OPTada_MemoryManager.h"

bool OPTada_MemoryManager::Init_Mamager(int Size_buff_)
{
	if (Size_buff_ < 1)
		return false;
	Size_buff_ += 1;
	Elem_buffer = (OPTada_Memory_S_MemoryManagerElement_Memory *)malloc(sizeof(OPTada_Memory_S_MemoryManagerElement_Memory) * Size_buff_); // получили пам€ть
	if (Elem_buffer == NULL)
		return false;
	Size_buff = Size_buff_;

	OPTada_Memory_S_MemoryManagerElement_Memory * new_elem_ = NULL;
	for (int i = 0; i < Size_buff; i++)
	{
		new_elem_ = &Elem_buffer[i];
		new_elem_->OPTada_Memory_S_MemoryManagerElement_Memory::OPTada_Memory_S_MemoryManagerElement_Memory(); // запуск конструкторов дл€ структур
	}	

	new_elem_ = &Elem_buffer[0];
	new_elem_->BufferType = 2; // установка дл€ первого буфера тип стандартного €чеистого
	new_elem_->Element_locked = true;
	new_elem_->Buffer = (OPTadaC_TemplateOfMemoryBuffer *)malloc(sizeof(OPTada_C_MultithreadedSimpleMemoryBuffer)); // запрос пам€ти
	// создали и инициализировали
	((OPTada_C_MultithreadedSimpleMemoryBuffer *)new_elem_->Buffer)->OPTada_C_MultithreadedSimpleMemoryBuffer::OPTada_C_MultithreadedSimpleMemoryBuffer(OPTada_Memory_DEF_StandartSizeNewBuffer, OPTada_Memory_DEF_StandartSizeNewBufferElems, OPTada_Memory_DEF_StandartSizeNewBufferCellSize);
	if (!new_elem_->Buffer->TestBuffer())
		return false;

	New_Buffer.type = 2;
	New_Buffer.nomer = 0;

	return true;
}

bool OPTada_MemoryManager::Free_Manager()
{
	OPTada_Memory_S_MemoryManagerElement_Memory * elem_ = NULL;
	for (int i = 0; i < Size_buff; i++)
	{
		elem_ = &Elem_buffer[i];
		if (elem_->Element_locked == true)
		{
			if (elem_->Buffer != NULL)
			{
				elem_->Buffer->Clear_Buffer();
				//TODO elem_->Buffer->~OPTada_Memory_C_TemplateMemoryBuffer(); //TODO ыыы // не корректный вызов???
				free(elem_->Buffer);
			}
			elem_->BufferType = 0;
			elem_->Element_locked = false;
		}
	}

	New_Buffer.type = -1;
	New_Buffer.nomer = -1;
	Size_buff = 0;
	if (Elem_buffer)
	{
		free(Elem_buffer);
	}

	return true;
}

OPTada_Memory_S_MemoryManedger * OPTada_MemoryManager::CreateNewBuffer(int type_, size_t Size_, size_t Elem_Buffer_Size_, size_t Cell_Size_)
{
	OPTada_Memory_S_MemoryManagerElement_Memory * elem_ = NULL;
	for (int i = 0; i < Size_buff; i++)
	{
		elem_ = &Elem_buffer[i];
		if (elem_->Element_locked == false)
		{
			elem_->Element_locked = true;
			elem_->BufferType = type_;
			switch (type_) // type
			{
			case 1:
			{
				// создание и сразу возврат указател€ + не забыть освободить синхронизацию потоков
			}
				break;
			case 2:
			{
				// создание многопоточного буфера и сразу возврат указател€ + не забыть освободить синхронизацию потоков
			}
				break;
			default:
			{
				elem_->BufferType = 0;
				elem_->Buffer = NULL;
				elem_->Element_locked = false;
				return NULL;
			}
				break;
			}
		}
	}
	return NULL;
}
