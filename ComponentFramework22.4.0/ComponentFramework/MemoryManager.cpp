#include "MemoryManager.h"

void* MemoryManager::allocate(std::size_t memorySize) {
	if (memorySize == 8) { //for the vector allocate
		return malloc(memorySize);
	}
	if (memorySize <= SIXTEEN_BYTES) { //16
		for (size_t iterator = 0; iterator < partitionSize - 19; iterator += 19) {
			void* preIterate = &masterBlock[iterator];
			char* guardByteStart = &(static_cast<char*>(preIterate)[0]) + 18;
			//std::cout << *guardByteStart << std::endl;
			if (*guardByteStart != 1) { //finish first available block
				*guardByteStart = 1; //block  unavailable

				nineteenByteList[iterator / 19] = preIterate; //add it to the nineteen byte list
				return preIterate;
			}
		}
	}
	else if (memorySize <= ONE_HUNDRED_BYTES) { //128
		for (size_t iterator = partitionSize; iterator < (partitionSize * 2) - 131; iterator += 131) {
			void* preIterate = &masterBlock[iterator];
			char* guardByteStart = &(static_cast<char*>(preIterate)[0]) + 130;
			if (*guardByteStart != 1) { //finish first available block
				*guardByteStart = 1; //block  unavailable

				oneHundredByteList[iterator / 131] = preIterate;
				return preIterate;
			}
		}
	}
	else if (memorySize <= ONE_THOUSAND_BYTES) { //1024
		for (size_t iterator = (partitionSize * 2); iterator < (partitionSize * 3) - 1027; iterator += 1027) {
			void* preIterate = &masterBlock[iterator];
			char* guardByteStart = &(static_cast<char*>(preIterate)[0]) + 1026;
			if (*guardByteStart != 1) { //finish first available block
				*guardByteStart = 1; //block  unavailable

				oneThousandByteList[iterator / 1027] = preIterate;
				return preIterate;
			}
		}
	}
	else if (memorySize <= EIGHT_THOUSAND_BYTES) { //8192
		for (size_t iterator = (partitionSize * 3); iterator < (partitionSize * 4) - 8195; iterator += 8195) {
			void* preIterate = &masterBlock[iterator];
			char* guardByteStart = &(static_cast<char*>(preIterate)[0]) + 8194;
			if (*guardByteStart != 1) { //finish first available block
				*guardByteStart = 1; //block  unavailable

				eightThousandByteList[iterator / 8195] = preIterate;
				return preIterate;
			}
		}
	}
	else if (memorySize <= THIRTY_TWO_THOUSAND_BYTES) { //32768
		for (size_t iterator = (partitionSize * 4); iterator < (partitionSize * 5) - 32771; iterator += 32771) {
			void* preIterate = &masterBlock[iterator];
			char* guardByteStart = &(static_cast<char*>(preIterate)[0]) + 32770;
			if (*guardByteStart != 1) { //finish first available block
				*guardByteStart = 1; //block  unavailable

				thirtyTwoThousandByteList[iterator / 32771] = preIterate;
				return preIterate;
			}
		}
	}
	else if (memorySize <= SIXTY_FIVE_THOUSAND_BYTES) { //65536
		for (size_t iterator = (partitionSize * 4); iterator < (partitionSize * 5) - 65539; iterator += 65539) {
			void* preIterate = &masterBlock[iterator];
			char* guardByteStart = &(static_cast<char*>(preIterate)[0]) + 65538;
			if (*guardByteStart != 1) { //finish first available block
				*guardByteStart = 1; //block  unavailable

				thirtyTwoThousandByteList[iterator / 65539] = preIterate;
				return preIterate;
			}
		}
	}
	//return malloc(memorySize);
	throw std::invalid_argument("Fatal Error: Can't Find a Memory Location");
	return nullptr;
	//return &masterBlock[blockIterator += memorySize];
}

void MemoryManager::deallocate(void* memoryLocation, std::size_t memorySize) {
	if (memorySize == 8) { //for the vector deallocate
		free(memoryLocation);
		return;
		//throw std::invalid_argument("Fatal Error: Can't Find Memory Location");
	}
	if (memorySize <= SIXTEEN_BYTES) { //16
		for (size_t x = 0; x < nineteenByteListSize; x++) { //loop through all the nineteen byte objects
			if (nineteenByteList[x] == memoryLocation) { //check if it == the memory location
				char* guardByteStart = &(static_cast<char*>(memoryLocation)[0]) + 16; //we should probably check the guard bits here
				*guardByteStart = 0xde;
				guardByteStart++;
				*guardByteStart = 0xad;   //end of block
				guardByteStart++;
				*guardByteStart = 0;      //block  available
				return;
			}
		}
	}
	else if (memorySize <= ONE_HUNDRED_BYTES) { //128
		for (size_t x = 0; x < oneHundredByteListSize; x++) { //loop through all the nineteen byte objects
			if (oneHundredByteList[x] == memoryLocation) { //check if it == the memory location
				char* guardByteStart = &(static_cast<char*>(memoryLocation)[0]) + 128; //we should probably check the guard bits here
				*guardByteStart = 0xde;
				guardByteStart++;
				*guardByteStart = 0xad;   //end of block
				guardByteStart++;
				*guardByteStart = 0;      //block  available
				return;
			}
		}
	}
	else if (memorySize <= ONE_THOUSAND_BYTES) { //1024
		for (size_t x = 0; x < oneThousandByteListSize; x++) { //loop through all the nineteen byte objects
			if (oneThousandByteList[x] == memoryLocation) { //check if it == the memory location
				char* guardByteStart = &(static_cast<char*>(memoryLocation)[0]) + 1024; //we should probably check the guard bits here
				*guardByteStart = 0xde;
				guardByteStart++;
				*guardByteStart = 0xad;   //end of block
				guardByteStart++;
				*guardByteStart = 0;      //block  available
				return;
			}
		}
	}
	else if (memorySize <= EIGHT_THOUSAND_BYTES) { //8192
		for (size_t x = 0; x < eightThousandByteListSize; x++) { //loop through all the nineteen byte objects
			if (eightThousandByteList[x] == memoryLocation) { //check if it == the memory location
				char* guardByteStart = &(static_cast<char*>(memoryLocation)[0]) + 8192; //we should probably check the guard bits here
				*guardByteStart = 0xde;
				guardByteStart++;
				*guardByteStart = 0xad;   //end of block
				guardByteStart++;
				*guardByteStart = 0;      //block  available
				return;
			}
		}
	}
	else if (memorySize <= THIRTY_TWO_THOUSAND_BYTES) { //32768
		for (size_t x = 0; x < thirtyTwoThousandByteListSize; x++) { //loop through all the nineteen byte objects
			if (thirtyTwoThousandByteList[x] == memoryLocation) { //check if it == the memory location
				char* guardByteStart = &(static_cast<char*>(memoryLocation)[0]) + 32768; //we should probably check the guard bits here
				*guardByteStart = 0xde;
				guardByteStart++;
				*guardByteStart = 0xad;   //end of block
				guardByteStart++;
				*guardByteStart = 0;      //block  available
				return;
			}
		}
	}
	else if (memorySize <= SIXTY_FIVE_THOUSAND_BYTES) { //65536
		for (size_t x = 0; x < sixtyFiveThousandByteListSize; x++) { //loop through all the nineteen byte objects
			if (thirtyTwoThousandByteList[x] == memoryLocation) { //check if it == the memory location
				char* guardByteStart = &(static_cast<char*>(memoryLocation)[0]) + 65536; //we should probably check the guard bits here
				*guardByteStart = 0xde;
				guardByteStart++;
				*guardByteStart = 0xad;   //end of block
				guardByteStart++;
				*guardByteStart = 0;      //block  available
				return;
			}
		}
	}
	else {
		throw std::invalid_argument("Fatal Error: Can't Find Memory Location");
		return;
	}
}

void MemoryManager::Initialize() {
	for (size_t iteratorNum = 0; iteratorNum < partitionSize - 19; iteratorNum += 19) { //loop through all objects and default there values
		void* iterator = &masterBlock[iteratorNum];
		char* guardByteStart = &(static_cast<char*>(iterator)[0]) + 16;
		*guardByteStart = 0xde;
		guardByteStart++;
		*guardByteStart = 0xad;   //end of block
		guardByteStart++;
		*guardByteStart = 0;      //block  available
		//std::cout << *guardByteStart << std::endl;
	}
	for (size_t iteratorNum = partitionSize; iteratorNum < (partitionSize * 2) - 131; iteratorNum += 131) { //loop through all objects and default there values
		void* iterator = &masterBlock[iteratorNum];
		char* guardByteStart = &(static_cast<char*>(iterator)[0]) + 128;
		*guardByteStart = 0xde;
		guardByteStart++;
		*guardByteStart = 0xad;   //end of block
		guardByteStart++;
		*guardByteStart = 0;      //block  available
	}
	for (size_t iteratorNum = (partitionSize * 2); iteratorNum < (partitionSize * 3) - 1027; iteratorNum += 1027) { //loop through all objects and default there values
		void* iterator = &masterBlock[iteratorNum];
		char* guardByteStart = &(static_cast<char*>(iterator)[0]) + 1024;
		*guardByteStart = 0xde;
		guardByteStart++;
		*guardByteStart = 0xad;   //end of block
		guardByteStart++;
		*guardByteStart = 0;      //block  available
	}
	for (size_t iteratorNum = (partitionSize * 3); iteratorNum < (partitionSize * 4) - 8195; iteratorNum += 8195) { //loop through all objects and default there values
		void* iterator = &masterBlock[iteratorNum];
		char* guardByteStart = &(static_cast<char*>(iterator)[0]) + 8192;
		*guardByteStart = 0xde;
		guardByteStart++;
		*guardByteStart = 0xad;   //end of block
		guardByteStart++;
		*guardByteStart = 0;      //block  available
	}
	for (size_t iteratorNum = (partitionSize * 4); iteratorNum < (partitionSize * 5) - 32771; iteratorNum += 32771) { //loop through all objects and default there values
		void* iterator = &masterBlock[iteratorNum];
		char* guardByteStart = &(static_cast<char*>(iterator)[0]) + 32768;
		*guardByteStart = 0xde;
		guardByteStart++;
		*guardByteStart = 0xad;   //end of block
		guardByteStart++;
		*guardByteStart = 0;      //block  available
	}
	for (size_t iteratorNum = (partitionSize * 5); iteratorNum < (partitionSize * 6) - 65539; iteratorNum += 65539) { //loop through all objects and default there values
		void* iterator = &masterBlock[iteratorNum];
		char* guardByteStart = &(static_cast<char*>(iterator)[0]) + 65536;
		*guardByteStart = 0xde;
		guardByteStart++;
		*guardByteStart = 0xad;   //end of block
		guardByteStart++;
		*guardByteStart = 0;      //block  available
	}
}

//std::shared_ptr<MemoryManager> MemoryManager::instance = 0;