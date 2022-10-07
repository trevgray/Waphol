#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include <string>
#include "EngineManager.h"
#include "Debug.h"
#include "MemoryManager.h"

static std::size_t memoryAllocated = 0;

static std::size_t maxMemoryAllocated = 0;
static std::size_t minMemoryAllocated = 0;

MemoryManager mm = MemoryManager(6000000);

void* operator new(std::size_t memorySize) {
	if (memorySize == 0) { memorySize++; }
	if (memorySize < minMemoryAllocated || minMemoryAllocated == 0) { minMemoryAllocated = memorySize; }
	if (memorySize > maxMemoryAllocated) { maxMemoryAllocated = memorySize; }
	//std::cout << "allocating " << memorySize << " bytes of memory\n";
	memoryAllocated += memorySize;

	return mm.allocate(memorySize);
}

void* operator new[](size_t memorySize)
{
	return mm.allocate(memorySize);
}

void operator delete[](void* memoryLocation, std::size_t memorySize)
{
	mm.deallocate(memoryLocation, memorySize);
}

void operator delete(void* memoryLocation, std::size_t memorySize) {
	//std::cout << "freeing " << memorySize << " bytes of memory\n";
	memoryAllocated -= memorySize;
	mm.deallocate(memoryLocation, memorySize);
}
  
int main(int argc, char* args[]) {
	
	/// Comment out the line below if you really want to make a 64-bit build
	static_assert(sizeof(void*) == 4, "Are you ready for 64-bit build?");

	Debug::DebugInit("GameEngineLog.txt"); //this makes a leak

	{ //generating a local scope so game gets destroyed before the dump memory
		if (EngineManager::Instance()->Initialize() != true) {

		}
	}

	EngineManager::Instance()->~EngineManager();

	//std::cout << memoryAllocated << std::endl;

	/// This writes out memory leaks to the output window not the console window
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	exit(0);
}