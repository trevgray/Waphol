#pragma once
#include "Actor.h"
#include <SDL_image.h>
#include <unordered_map>
#include <SDL_gamecontroller.h>

struct GameController {
	GameController() {
		SDLController = nullptr;
	}
	/*Controller(SDL_GameController* controllerPtr) { 
		SDLController = controllerPtr;
	};*/
	SDL_GameController* SDLController;
	//Ref<SDL_Joystick> SDLStick0;
	//Ref<SDL_Joystick> SDLStick1;
	~GameController() {
		SDL_GameControllerClose(SDLController);
		
		std::cout << "TEST\n"; }
};

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
	std::vector<Ref<GameController>> gameControllers;
};

