#define FREE_BLOCKS_LIST_H
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
				FreeMemoryBlock *deletePtr = traversalPtr->getNext();

				traversalPtr->setNextBlock(deletePtr->getNextBlock());
				traversalPtr->setEndAddress(deletePtr->getEndAddress());	//merge the two blocks
				delete(deletePtr);

			}else{
				traversalPtr = traversalPtr->getNextBlock();
			} 
		}		
	}


public:
	LinkedList(){
		head = NULL;
	}

	// this method adds memory block in sorted order and merges the free blocks afterwards
	void addMemoryBlock(FreeMemoryBlock *freeMemoryBlock){
		if(head == NULL){
			head = freeMemoryBlock;
			return;
		}

		FreeMemoryBlock *traversalPtr = head;

		while(traversalPtr->getNextBlock() && traversalPtr->getNextBlock()->getStartAddress() < freeMemoryBlock->getStartAddress()){
			traversalPtr = traversalPtr->getNextBlock();
		}

		freeMemoryBlock->setNextBlock(traversalPtr->getNextBlock()->getNextBlock());
		traversalPtr->setNextBlock(FreeMemoryBlock);

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

			if((traversalPtr->getEndAddress() - traversalPtr->getStartAddress()) >= 0){
				currentDelta = traversalPtr->getEndAddress() - traversalPtr->getStartAddress();
			}

			if(currentDelta < delta){
				delta = currentDelta;
				previousToBestFittingBlock = previous
				bestFittingBlock = traversalPtr;
			}

			previous = traversalPtr;
			traversalPtr = traversalPtr->getNextBlock();
		}

		//now remove the best fitting block
		if(bestFittingBlock){
			previousToBestFittingBlock->setNextBlock(bestFittingBlock->getNext());
			newBlock = new AllocatedMemoryBlock(processId, bestFittingBlock->getStartAddress(), bestFittingBlock->getEndAddress());
		}

		delete(bestFittingBlock);

		return newBlock;

	}

	// first fit

	void allocateFirstFittingBlock(double size){

		AllocatedMemoryBlock *newBlock = NULL;

		FreeMemoryBlock *traversalPtr = head;
		FreeMemoryBlock *previous = NULL
		FreeMemoryBlock *firstFittingBlock = NULL;

		while(traversalPtr){
			if((traversalPtr->getEndAddress() - traversalPtr->getStartAddress()) >= 0){
				firstFittingBlock = traversalPtr;
			}

			previous = traversalPtr;
			traversalPtr = traversalPtr->getNextBlock();
		}

		//now delete the firstFittingBlock
		if(firstFittingBlock){
			previous->setNextBlock(firstFittingBlock->getNextBlock());
			newBlock = new AllocatedMemoryBlock(processId, firstFittingBlock->getStartAddress(), firstFittingBlock->getEndAddress());
			delete(bestFittingBlock);	
		}

		return newBlock;
		
	}

}