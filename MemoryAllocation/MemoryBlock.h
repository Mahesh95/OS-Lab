#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

class MemoryBlock{
private:

	double startAddress;
	double endAddress;

public:

	MemoryBlock(double startAddress, double endAddress){
		this->startAddress = startAddress;
		this->endAddress = endAddress;
	}

	double getStartAddress(){
		return this->startAddress;
	}

	double getEndAddress(){
		return this->endAddress;
	}

	void setStartAddress(double address){
		this->startAddress = address;
	}

	void setEndAddress(double address){
		this->endAddress = address;
	}

	virtual MemoryBlock* getNextBlock() = 0;
	virtual void setNextBlock(MemoryBlock*){
		
	}

};

#endif
