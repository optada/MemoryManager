// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_SimpleMemoryBuffer.h"


inline void* OPTadaC_SimpleMemoryBuffer::TakeMemoryMethod(size_t new_size_)
{
	OPTadaS_MemoryCell_Element* cell_elem = freeCells_Buffer;
	if (new_size_ < 1 || (new_size_ + cellOfDefragmentation_Size) < new_size_) {
		return NULL;
	}

	// adding a coefficient to the size(to reduce defragmentation)
	size_t test_cell_size = new_size_ % cellOfDefragmentation_Size;
	if (test_cell_size) {
		new_size_ += cellOfDefragmentation_Size - test_cell_size;
	}

	// do we have enaght memory?
	if (buffer_Length - lockedMemory < new_size_) {
		return NULL;
	}

	while (freeCells_Buffer) {

		if (cell_elem->size >= new_size_) { // found the right size, start sharing ...
			if (cell_elem->size == new_size_) { // if the cell is the same size

				// cut it from the free buffer
				CutCellFromTheBuffer(cell_elem, &freeCells_Buffer);

				// saving the size of occupied memory
				lockedMemory += new_size_;
				cell_elem->isfree = false;

				// transfer to blocked
				TransferCellToBuffer(cell_elem, &lockedCells_Buffer);

				return cell_elem->link;
			}
			else { // cell larger - trim the free cell and create a lock cell

				// added a new cell 
				OPTadaS_MemoryCell_Element* new_elem = cellBuffer->Get_Element();
				if (new_elem == NULL) {
					return NULL;
				}

				// include new cell to mass
				if (cell_elem->previous_el) { // there is a previous cell
					new_elem->previous_el = cell_elem->previous_el;
					new_elem->next_el = cell_elem;
					new_elem->previous_el->next_el = new_elem;
					cell_elem->previous_el = new_elem;
				}
				else { 
					new_elem->next_el = cell_elem;
					cell_elem->previous_el = new_elem;
					new_elem->previous_el = NULL;
					firstCell_Buffer = new_elem;
				}
				
				// separate the piece and change the links in the main element
				cell_elem->size = (cell_elem->size - new_size_);
				new_elem->size = new_size_;
				new_elem->link = cell_elem->link;
				cell_elem->link = &cell_elem->link[new_size_];

				// saving the size of occupied memory
				lockedMemory += new_size_;
				new_elem->isfree = false;

				// transfer to blocked
				TransferCellToBuffer(new_elem, &lockedCells_Buffer);

				return new_elem->link;
			}			
		}
		else { // looking next
			if (cell_elem->next_link) {
				cell_elem = cell_elem->next_link;
			}
			else {
				return NULL;
			}
		}
	}

	return NULL;
}

inline void OPTadaC_SimpleMemoryBuffer::CutCellFromTheBuffer(OPTadaS_MemoryCell_Element* cell_elem_, OPTadaS_MemoryCell_Element** buffer_)
{
	// cut out of free buffer
	if (cell_elem_->previous_link) { // there is an element (closer to the beginning)
		if (cell_elem_->next_link) { // there is the following element
			cell_elem_->next_link->previous_link = cell_elem_->previous_link;
			cell_elem_->previous_link->next_link = cell_elem_->next_link;
		}
		else { // нету следующего элемента
			cell_elem_->previous_link->next_link = NULL;
		}
	}
	else { // no element(closer to the beginning)
		if (cell_elem_->next_link) { // there is the following element
			(*buffer_) = cell_elem_->next_link;
			cell_elem_->next_link->previous_link = NULL;
		}
		else { // no next item
			(*buffer_) = NULL;
		}
	}
}

inline void OPTadaC_SimpleMemoryBuffer::TransferCellToBuffer(OPTadaS_MemoryCell_Element* cell_elem_, OPTadaS_MemoryCell_Element** buffer_)
{
	if ((*buffer_)) { // if buffer not NULL - include
		(*buffer_)->previous_link = cell_elem_;
		cell_elem_->previous_link = NULL;
		cell_elem_->next_link = (*buffer_);
		(*buffer_) = cell_elem_;
	}
	else { // if buffer = NULL - just add 
		cell_elem_->next_link = NULL;
		cell_elem_->previous_link = NULL;
		(*buffer_) = cell_elem_;
	}
}

inline bool OPTadaC_SimpleMemoryBuffer::TryMergeNeighboringFreeCells(OPTadaS_MemoryCell_Element* cell_elem_)
{
	OPTadaS_MemoryCell_Element* dell_elem = NULL;

	// search for an item on the left to merge(previous)
	if (cell_elem_->previous_el) { // got left cell
		if (cell_elem_->previous_el->isfree) { // the preceding element is ready to merge

			cell_elem_->previous_el->next_el = cell_elem_->next_el;
			if (cell_elem_->next_el != NULL) {
				cell_elem_->next_el->previous_el = cell_elem_->previous_el;
			}

			cell_elem_->previous_el->size += cell_elem_->size; // changed the total size
			dell_elem = cell_elem_;
			cell_elem_ = cell_elem_->previous_el;

			// cut out from the free buffer
			CutCellFromTheBuffer(dell_elem, &freeCells_Buffer);
			cellBuffer->Return_Element(dell_elem); // delete cell
		}
	}

	if (cell_elem_->next_el) { // search for the item on the right to merge (next)
		if (cell_elem_->next_el->isfree) { // the next item is ready to merge

			cell_elem_ = cell_elem_->next_el; // took the link and now we are working with the next element

			cell_elem_->previous_el->next_el = cell_elem_->next_el;
			if (cell_elem_->next_el != NULL) {
				cell_elem_->next_el->previous_el = cell_elem_->previous_el;
			}

			cell_elem_->previous_el->size += cell_elem_->size; // changed the total size
			dell_elem = cell_elem_;
			cell_elem_ = cell_elem_->previous_el;

			// cut out from the free buffer
			CutCellFromTheBuffer(dell_elem, &freeCells_Buffer);
			cellBuffer->Return_Element(dell_elem); // delete cell
		}
	}

	return dell_elem;
}


OPTadaC_SimpleMemoryBuffer::OPTadaC_SimpleMemoryBuffer(size_t size_, size_t cellBuffer_Size_, size_t cellOfDefragmentation_Size_, bool& initDoneWithNoErrors_)
{
	// malloc memory for buffer
	buffer = (char*)malloc(size_); 
	if (!buffer) {
		initDoneWithNoErrors_ = false;
		return;
	}

	// create support class-buffer (with cells)
	cellBuffer = (OPTadaC_MemoryCells_StaticCyclicBuffer*)malloc(sizeof(OPTadaC_MemoryCells_StaticCyclicBuffer)); 
	if (!cellBuffer) {
		initDoneWithNoErrors_ = false;
		return;
	}
	cellBuffer->OPTadaC_MemoryCells_StaticCyclicBuffer::OPTadaC_MemoryCells_StaticCyclicBuffer(cellBuffer_Size_, initDoneWithNoErrors_);
	if (!initDoneWithNoErrors_) {
		return;
	}

	// set parameters
	buffer_Length = size_;
	lockedMemory = 0;
	lockedCells_Buffer = NULL;
	cellOfDefragmentation_Size = cellOfDefragmentation_Size_;

	// setting up first memory cell
	freeCells_Buffer = cellBuffer->Get_Element();
	firstCell_Buffer = freeCells_Buffer;

	if (freeCells_Buffer) {
		freeCells_Buffer->link = buffer;
		freeCells_Buffer->next_el = NULL;
		freeCells_Buffer->previous_el = NULL;
		freeCells_Buffer->next_link = NULL;
		freeCells_Buffer->previous_link = NULL;
		freeCells_Buffer->size = size_;
	}
	else {
		initDoneWithNoErrors_ = false;
		return;
	}

	initDoneWithNoErrors_ = TestBuffer();
}

OPTadaC_SimpleMemoryBuffer::~OPTadaC_SimpleMemoryBuffer()
{
	if (cellBuffer != NULL) {
		cellBuffer->~OPTadaC_MemoryCells_StaticCyclicBuffer();
		free(cellBuffer);
		cellBuffer = NULL;
	}

	if (buffer != NULL) {
		free(buffer);
	}
}

bool OPTadaC_SimpleMemoryBuffer::Clear_Buffer()
{
	// we go through all the cells and return them
	OPTadaS_MemoryCell_Element * cellDell = firstCell_Buffer->next_el; // second cell

	// have no buffer of cells ERROR
	if (!cellBuffer) { 
		return false;
	}

	while (cellDell) {		

		if (cellDell->next_el) { // if we have next cell
			cellDell = cellDell->next_el;
			cellBuffer->Return_Element(cellDell->previous_el);
		}
		else { // this cell is last
			cellBuffer->Return_Element(cellDell);
			cellDell = NULL;
		}
	}

	// setting up buffers again
	lockedCells_Buffer = NULL;
	freeCells_Buffer = firstCell_Buffer;
	lockedMemory = 0;

	// setting up first cell again
	if (freeCells_Buffer) {
		freeCells_Buffer->isfree = true;
		freeCells_Buffer->link = buffer;
		freeCells_Buffer->next_el = NULL;
		freeCells_Buffer->previous_el = NULL;
		freeCells_Buffer->next_link = NULL;
		freeCells_Buffer->previous_link = NULL;
		freeCells_Buffer->size = buffer_Length;
	}

	return true;
}

void* OPTadaC_SimpleMemoryBuffer::GetMemory(size_t new_Length_)
{
	return TakeMemoryMethod(new_Length_); // main algoritm
}

bool OPTadaC_SimpleMemoryBuffer::ReturnMemory(void* link_)
{
	if (link_ != NULL && (link_ >= buffer && link_ <= &buffer[buffer_Length])) { 
		// reference to our buffer range
		OPTadaS_MemoryCell_Element* cell_elem = lockedCells_Buffer;
		while (cell_elem) {

			if (cell_elem->link == link_) { // found 
				
				// Cut cell from the locked buffer
				CutCellFromTheBuffer(cell_elem, &lockedCells_Buffer);

				cell_elem->next_link = NULL;
				cell_elem->previous_link = NULL;
				lockedMemory -= cell_elem->size; // freed up size
				cell_elem->isfree = true;

				// looking for neighbors to merge - kill sam sel f :(
				bool mergerWasMade = TryMergeNeighboringFreeCells(cell_elem);

				// if we can't merge - we push cell in free
				if (!mergerWasMade) { 
					TransferCellToBuffer(cell_elem, &freeCells_Buffer);
				}

				return true;
			}
			else { // not found - next
				cell_elem = cell_elem->next_link;
			}
		}

		return false;
	}
	else { // not our reference
		return false;
	}
}

bool OPTadaC_SimpleMemoryBuffer::TestBuffer()
{
	if (buffer != NULL && buffer_Length > 0 && firstCell_Buffer != NULL && freeCells_Buffer != NULL 
		&& cellBuffer != NULL && cellOfDefragmentation_Size > 0) {
		return true;
	}
	else {
		return false;
	}
}

size_t OPTadaC_SimpleMemoryBuffer::Get_LockedMemory()
{
	if (buffer_Length > 0) {
		return lockedMemory;
	}
	else {
		return 0;
	}
}

size_t OPTadaC_SimpleMemoryBuffer::Get_AllModulesLockedMemory()
{
	if (buffer_Length > 0) {
		return (cellBuffer->Get_AllCapturedMemory() + buffer_Length + sizeof(OPTadaC_SimpleMemoryBuffer));
	}
	else {
		return 0;
	}
}

size_t OPTadaC_SimpleMemoryBuffer::Get_BufferMemorySize()
{
	return buffer_Length;
}
