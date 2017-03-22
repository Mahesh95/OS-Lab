#ifndef FREE_MEMORY_BLOCK_H
#define FREE_MEMORY_BLOCK_H

#include "MemoryBlock.h"

class FreeMemoryBlock:public MemoryBlock{
private:
	
	FreeMemoryBlock *next;

public:
	
	FreeMemoryBlock(double startAddress, double endAddress)
	:MemoryBlock(startAddress, endAddress){
		this->next = NULL:
	}

	virtual FreeMemoryBlock *getNextBlock(){
		return this->next;
	}	

	virtual void setNextBlock(FreeMemoryBlock *freeMemoryBlock){
		this->next = freeMemoryBlock;
	}
}