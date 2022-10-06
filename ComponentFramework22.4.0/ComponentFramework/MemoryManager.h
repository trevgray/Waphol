#pragma once
#include <memory>
#include <iostream>

//struct MemorySegment {
//	MemorySegment() {
//		memoryLocation = nullptr;
//		next = nullptr;
//	}
//	MemorySegment(void* memoryLocation_) {
//		memoryLocation = memoryLocation_;
//		next = nullptr;
//	}
//	/*void SetNext(void* next_) {
//		next = next_;
//	}*/
//	void* memoryLocation;
//	void* next;
//};

const int SIXTEEN_BYTES = 16;
const int ONE_HUNDRED_BYTES = 128;
const int ONE_THOUSAND_BYTES = 1024;
const int EIGHT_THOUSAND_BYTES = 8192;
const int THIRTY_TWO_THOUSAND_BYTES = 32768;

using byte = unsigned char; //basically just 8 bytes
	class MemoryManager { //https://developer.ibm.com/tutorials/au-memorymanager/
	private:
		//static std::shared_ptr<MemoryManager> instance;
		byte* masterBlock; //collection of bytes

		std::size_t partitionSize;

		int nineteenByteListSize;
		int oneHundredByteListSize;
		int oneThousandByteListSize;
		int eightThousandByteListSize;
		int thirtyTwoThousandByteListSize;

		void* nineteenByteList[52631]; //partitionSize / 16 + 3
		void* oneHundredByteList[7633]; //partitionSize / 128 + 3
		void* oneThousandByteList[973]; //partitionSize / 1024 + 3
		void* eightThousandByteList[122]; //partitionSize / 8192 + 3
		void* thirtyTwoThousandByteList[29]; //partitionSize / 32768 + 3

	public:
		MemoryManager(std::size_t masterBlockSize) :masterBlock(nullptr) { //set up for 5000000 bits
			masterBlock = static_cast<byte*>(malloc(masterBlockSize));
			partitionSize = masterBlockSize / 5; //making 5 

			nineteenByteListSize = 52631; //same as the array
			oneHundredByteListSize = 7633;
			oneThousandByteListSize = 973;
			eightThousandByteListSize = 122;
			thirtyTwoThousandByteListSize = 29;

			Initialize();
			/*sixteenByteBlockIterator = 0;
			oneHundredByteBlockIterator = partitionSize;
			oneThousandByteBlockIterator = (partitionSize * 2);
			eightThousandByteBlockIterator = (partitionSize * 3);
			thirtyTwoThousandByteBlockIterator = (partitionSize * 4);*/

			std::cout << "Master block is " << masterBlockSize << " bytes" << std::endl;
		}
		/*static std::shared_ptr<MemoryManager> GetInstance() {
			if (!instance) {
				instance = std::shared_ptr<MemoryManager>(new MemoryManager);
			}
			return instance;
		}*/

		~MemoryManager() {
			if (masterBlock) free(masterBlock);
			std::cout << "Freed the MasterBlock" << std::endl;
		}
		void* allocate(std::size_t memorySize);
		void deallocate(void* memoryLocation, std::size_t memorySize);
		void Initialize();
	};

