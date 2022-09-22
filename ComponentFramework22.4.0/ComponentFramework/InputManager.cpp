#include "InputManager.h"
#include "ControllerComponent.h"

InputManager::InputManager() {}

InputManager::~InputManager() {
	controllerActors.clear();
}

void InputManager::HandleInputs(const SDL_Event& sdlEvent) {
	for (auto actor : controllerActors) {
		std::string actorName;
		actorName = actor->GetComponent<ControllerComponent>()->GetControllerTemplate();
		if (actorName == "PlayerController") {
            PlayerController(sdlEvent);
		}
	}
}

void InputManager::SetControllerActors(std::unordered_map<std::string, Ref<Actor>> actorGraph) {
	for (auto actor : actorGraph) {
		if (actor.second->GetComponent<ControllerComponent>() != nullptr) {
			controllerActors.push_back(actor.second);
		}
	}
}

void InputManager::PlayerController(const SDL_Event& sdlEvent) { //temp controller class till i figure out a better system
    // if key pressed, set velocity or acceleration

    if (sdlEvent.type == SDL_KEYDOWN && sdlEvent.key.repeat == 0)
    {
        switch (sdlEvent.key.keysym.scancode)
        {
            // This section demonstrates using velocity for player movement
            //
            // Need to always normalize speed, otherwise having two keys down
            // results in velocity magnitude being sqrt(2) x maxSpeed.
            // However, this is being done in Update()

        /*case SDL_SCANCODE_W:
            vel.y = maxSpeed * 1.0f;
            break;
        case SDL_SCANCODE_A:
            vel.x = maxSpeed * -1.0f;
            break;
        case SDL_SCANCODE_S:
            vel.y = maxSpeed * -1.0f;
            break;
        case SDL_SCANCODE_D:
            vel.x = maxSpeed * 1.0f;
            break;*/

            // This section is for seeing how to use acceleration rather than velocity
            // for player movement.
            // Note: look at Update() to see that velocity is clipped, since
            // continuous acceleration means continually increasing velocity.

        /*case SDL_SCANCODE_DOWN:
            accel.y = maxAcceleration * -1.0f;
            break;
        case SDL_SCANCODE_UP:
            accel.y = maxAcceleration * 1.0f;
            break;
        case SDL_SCANCODE_LEFT:
            accel.x = maxAcceleration * -1.0f;
            break;
        case SDL_SCANCODE_RIGHT:
            accel.x = maxAcceleration * 1.0f;
            break;
        default:
            break;*/
        }
    }

    // if key is released, stop applying movement

    if (sdlEvent.type == SDL_KEYUP && sdlEvent.key.repeat == 0)
    {
        switch (sdlEvent.key.keysym.scancode)
        {
            // This section demonstrates using velocity for player movement
            //
            // Need to always normalize velocity, otherwise if player
            // releases one of two pressed keys, then speed remains at sqrt(0.5) of maxSpeed

        /*case SDL_SCANCODE_W:
            vel.y = 0.0f;
            if (VMath::mag(vel) > VERY_SMALL) vel = VMath::normalize(vel) * maxSpeed;
            break;
        case SDL_SCANCODE_A:
            vel.x = -0.0f;
            if (VMath::mag(vel) > VERY_SMALL) vel = VMath::normalize(vel) * maxSpeed;
            break;
        case SDL_SCANCODE_S:
            vel.y = -0.0f;
            if (VMath::mag(vel) > VERY_SMALL) vel = VMath::normalize(vel) * maxSpeed;
            break;
        case SDL_SCANCODE_D:
            vel.x = 0.0f;
            if (VMath::mag(vel) > VERY_SMALL) vel = VMath::normalize(vel) * maxSpeed;
            break;*/

            // This section is for seeing how to use acceleration rather than velocity
            // for player movement.

        /*case SDL_SCANCODE_DOWN:
            accel.y = 0.0;
            break;
        case SDL_SCANCODE_UP:
            accel.y = 0.0;
            break;
        case SDL_SCANCODE_LEFT:
            accel.x = 0.0;
            break;
        case SDL_SCANCODE_RIGHT:
            accel.x = 0.0;
            break;
        default:
            break;*/
        }
    }
}