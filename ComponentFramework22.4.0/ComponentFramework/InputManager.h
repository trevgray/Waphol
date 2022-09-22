#pragma once
#include "Actor.h"
#include <SDL_image.h>
#include <unordered_map>

class InputManager {
public:
	InputManager();
	~InputManager();

	void HandleInputs(const SDL_Event& sdlEvent);

	void SetControllerActors(std::unordered_map <std::string, Ref<Actor>> actorGraph);

	//Controllers
	void PlayerController(const SDL_Event& sdlEvent, Ref<Actor> actor);

private:
	std::vector <Ref<Actor>> controllerActors;
};

