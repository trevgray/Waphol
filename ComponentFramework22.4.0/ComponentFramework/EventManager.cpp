#include "EventManager.h"
#include "EngineManager.h"

EventManager::EventManager() {
	sdlEvent = SDL_Event();
}

EventManager::~EventManager() {}

void EventManager::Run() {
	//while (EngineManager::Instance()->GetIsRunning() == true) {
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
				EngineManager::Instance()->SetIsRunning(false);
				return;
			}
			else if (sdlEvent.type == SDL_KEYDOWN) {
				switch (sdlEvent.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					/*case SDL_SCANCODE_Q:
						isRunning = false;
						return;
						[[fallthrough]]; /// C17 Prevents switch/case fallthrough warnings*/
				default:
					break;
				}
			}
			/*if (EngineManager::Instance()->GetSceneManager()->GetCurrentScene() == nullptr) {
				Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
				EngineManager::Instance()->SetIsRunning(false);
				return;
			}*/
			EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
			//EngineManager::Instance()->GetSceneManager()->GetCurrentScene()->HandleEvents(sdlEvent);
		}
	//}
}