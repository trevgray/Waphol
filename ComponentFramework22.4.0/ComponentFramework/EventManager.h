#pragma once
#include <SDL_events.h>

class EventManager {
public:
	EventManager();
	~EventManager();
	void Run();
private:
	SDL_Event sdlEvent;
};

