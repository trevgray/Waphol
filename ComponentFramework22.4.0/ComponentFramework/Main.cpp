#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include <memory>
#include <string>
#include "SceneManager.h"
#include "Debug.h"

  
int main(int argc, char* args[]) {
	
	/// Comment out the line below if you really want to make a 64-bit build
	static_assert(sizeof(void*) == 4, "Are you ready for 64-bit build?");

	Debug::DebugInit("GameEngineLog.txt");

	{ //generating a local scope so game gets destroyed before the dump memory
		std::unique_ptr<SceneManager> game = std::make_unique<SceneManager>();
		if (game->Initialize("Game Engine", 1280, 720) == true) {
			game->Run();
		}
	}
	/// This writes out memory leaks to the output window not the console window
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	exit(0);
}