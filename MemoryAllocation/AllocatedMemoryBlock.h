#ifndef ALLOCATED_MEMORY_BLOCK_H
#define ALLOCATED_MEMORY_BLOCK_H

#include "MemoryBlock.h"

class AllocatedMemoryBlock:public MemoryBlock{
private:
	double processId;
	AllocatedMemoryBlock *next;

public:
	AllocatedMemoryBlock(double processId, double startAddress, double endAddress)
	:MemoryBlock(startAddress, endAddress){
		this->processId = processId;
		this->next = NULL;
	}

	double getProcessId(){
		return this->processId;
	}

	virtual AllocatedMemoryBlock* getNextBlock(){
		return this->next;
	}

	virtual void setNextBlock(AllocatedMemoryBlock *allocatedMemoryBlock){
		this->next = allocatedMemoryBlock;
	}

};

#endif