#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include <string>
#include "EngineManager.h"
#include "Debug.h"
  
int main(int argc, char* args[]) {
	
	/// Comment out the line below if you really want to make a 64-bit build
	static_assert(sizeof(void*) == 4, "Are you ready for 64-bit build?");

	Debug::DebugInit("GameEngineLog.txt"); //this makes a leak

	{ //generating a local scope so game gets destroyed before the dump memory
		if (EngineManager::Instance()->Initialize() != true) {

		}
	}

	EngineManager::Instance()->~EngineManager();

	/// This writes out memory leaks to the output window not the console window
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	exit(0);
}