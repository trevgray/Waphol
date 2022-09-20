#pragma once

#include "Actor.h"
#include "EngineManager.h"
#include "CameraActor.h"
#include "LightActor.h"
#include <unordered_map>

union SDL_Event;

class Scene {
public:
	virtual ~Scene() {};
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
};