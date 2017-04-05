#ifndef FREE_BLOCKS_LIST_H
#define FREE_BLOCKS_LIST_H

#include "MemoryBlock.h"
#include "AllocatedMemoryBlock.h"

#define MAX 100000

class FreeBlocksList{
private:
	FreeMemoryBlock *head;


	//this method merges the free memory blocks

	void mergeFreeBlocks(){
		FreeMemoryBlock *traversalPtr = head;

		while(traversalPtr && traversalPtr->getNextBlock()){

			if((traversalPtr->getEndAddress()+1) == traversalPtr->getNextBlock()->getStartAddress()){
				FreeMemoryBlock *deletePtr = traversalPtr->getNextBlock();

				traversalPtr->setNextBlock(deletePtr->getNextBlock());
				traversalPtr->setEndAddress(deletePtr->getEndAddress());	//merge the two blocks
				delete(deletePtr);

			}else{
				traversalPtr = traversalPtr->getNextBlock();
			} 
		}		
	}


public:
	FreeBlocksList(){
		head = NULL;
	}

	// this method adds memory block in sorted order and merges the free blocks afterwards
	void addMemoryBlock(FreeMemoryBlock *freeMemoryBlock){
		if(head == NULL){
			head = freeMemoryBlock;
			return;
		}

		// if block to be added has smallest start address
		if(freeMemoryBlock->getStartAddress() < head->getStartAddress()){
			freeMemoryBlock->setNextBlock(head);
			head = freeMemoryBlock;
			mergeFreeBlocks();
			return;
		}

		FreeMemoryBlock *traversalPtr = head;

		while(traversalPtr->getNextBlock() && traversalPtr->getNextBlock()->getStartAddress() < freeMemoryBlock->getStartAddress()){
			traversalPtr = traversalPtr->getNextBlock();
		}

		freeMemoryBlock->setNextBlock(traversalPtr->getNextBlock());
		traversalPtr->setNextBlock(freeMemoryBlock);

		mergeFreeBlocks();
	}


	//returns the newly created allocatedMemoryBlock
	AllocatedMemoryBlock *allocateBestFittingBlock(double processId, double size){
		
		AllocatedMemoryBlock *newBlock = NULL;

		double delta = MAX;	// delta is the difference betweeen required sized and the size of block

		FreeMemoryBlock *traversalPtr = head;
		FreeMemoryBlock *previous = NULL;
		FreeMemoryBlock *bestFittingBlock = NULL;
		FreeMemoryBlock *previousToBestFittingBlock = NULL;

		while(traversalPtr){
			double currentDelta = MAX;

			if((traversalPtr->getEndAddress() - traversalPtr->getStartAddress() + 1) >= size){
				currentDelta = (traversalPtr->getEndAddress() - traversalPtr->getStartAddress()+1) - size;
			}

			if(currentDelta < delta){
				delta = currentDelta;
				previousToBestFittingBlock = previous;
				bestFittingBlock = traversalPtr;
			}

			previous = traversalPtr;
			traversalPtr = traversalPtr->getNextBlock();
		}

		//now remove the best fitting block
		if(bestFittingBlock){
			if(bestFittingBlock == head){
				head = head->getNextBlock();
			}else{
				previousToBestFittingBlock->setNextBlock(bestFittingBlock->getNextBlock());	
			}
			
			newBlock = new AllocatedMemoryBlock(processId, bestFittingBlock->getStartAddress(), bestFittingBlock->getStartAddress() + size-1);
		}
		if(size < bestFittingBlock->getEndAddress() - bestFittingBlock->getStartAddress()){
			double fragmentedBlockStartAddress = bestFittingBlock->getStartAddress()+size;
			double fragmentedBlockEndAddress = bestFittingBlock->getEndAddress();
			delete(bestFittingBlock);

			FreeMemoryBlock* fragmentedBlock = new FreeMemoryBlock(fragmentedBlockStartAddress, fragmentedBlockEndAddress);	
			this->addMemoryBlock(fragmentedBlock);
		}else{
			delete bestFittingBlock;
		}
		
		return newBlock;

	}

	// first fit

	AllocatedMemoryBlock* allocateFirstFittingBlock(double processId, double size){

		AllocatedMemoryBlock *newBlock = NULL;

		FreeMemoryBlock *traversalPtr = head;
		FreeMemoryBlock *previous = NULL;
		FreeMemoryBlock *firstFittingBlock = NULL;

		while(traversalPtr){
			if((traversalPtr->getEndAddress() - traversalPtr->getStartAddress()+1) >= size){
				firstFittingBlock = traversalPtr;
				break;
			}

			previous = traversalPtr;
			traversalPtr = traversalPtr->getNextBlock();
		}


		//now delete the firstFittingBlock
		if(firstFittingBlock){
			if(firstFittingBlock == head){
				head = head->getNextBlock();
			}else{
				previous->setNextBlock(firstFittingBlock->getNextBlock());	
			}
			
			newBlock = new AllocatedMemoryBlock(processId, firstFittingBlock->getStartAddress(), firstFittingBlock->getStartAddress()+size-1);

			if(size < firstFittingBlock->getEndAddress() - firstFittingBlock->getStartAddress()){
				double fragmentedBlockStartAddress = firstFittingBlock->getStartAddress()+size;
				double fragmentedBlockEndAddress = firstFittingBlock->getEndAddress();
				delete(firstFittingBlock);

				FreeMemoryBlock* fragmentedBlock = new FreeMemoryBlock(fragmentedBlockStartAddress, fragmentedBlockEndAddress);
				this->addMemoryBlock(fragmentedBlock);
			}else{
				delete(firstFittingBlock);	
			}			
		}

		return newBlock;
		
	}

AllocatedMemoryBlock* allocateWorstFittingBlock(double processId, double size){
		AllocatedMemoryBlock *newBlock = NULL;

		double delta = 0;	// delta is the difference betweeen required sized and the size of block

		FreeMemoryBlock *traversalPtr = head;
		FreeMemoryBlock *previous = NULL;
		FreeMemoryBlock *worstFittingBlock = NULL;
		FreeMemoryBlock *previousToWorstFittingBlock = NULL;

		while(traversalPtr){
			double currentDelta = 0;

			if((traversalPtr->getEndAddress() - traversalPtr->getStartAddress()) >= size){
				currentDelta = (traversalPtr->getEndAddress() - traversalPtr->getStartAddress()) - size;
			}

			if(currentDelta > delta){
				delta = currentDelta;
				previousToWorstFittingBlock = previous;
				worstFittingBlock = traversalPtr;
			}

			previous = traversalPtr;
			traversalPtr = traversalPtr->getNextBlock();
		}

		//now remove the best fitting block
		if(worstFittingBlock){
			if(worstFittingBlock == head){
				head = head->getNextBlock();
			}else{
				previousToWorstFittingBlock->setNextBlock(worstFittingBlock->getNextBlock());	
			}
			
			newBlock = new AllocatedMemoryBlock(processId, worstFittingBlock->getStartAddress(), worstFittingBlock->getStartAddress() + size-1);
		}
		if(size < worstFittingBlock->getEndAddress() - worstFittingBlock->getStartAddress()){
			double fragmentedBlockStartAddress = worstFittingBlock->getStartAddress()+size-1;
			double fragmentedBlockEndAddress = worstFittingBlock->getEndAddress();
			delete(worstFittingBlock);

			FreeMemoryBlock* fragmentedBlock = new FreeMemoryBlock(fragmentedBlockStartAddress, fragmentedBlockEndAddress);	
			this->addMemoryBlock(fragmentedBlock);
		}else{
			delete worstFittingBlock;
		}
		
		return newBlock;
}

	void printList(){
		FreeMemoryBlock* traversalPtr = head;
		int count = 0;
		while(traversalPtr){
			std::cout << "block no " << count+1 << " " << traversalPtr->getStartAddress() << " " << traversalPtr->getEndAddress() << std::endl;
			traversalPtr = traversalPtr->getNextBlock();
			count++;
		}
	}

};

#endif