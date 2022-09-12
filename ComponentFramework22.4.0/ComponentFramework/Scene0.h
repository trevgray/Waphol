#pragma once
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "Actor.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene0 : public Scene {
private:
	CameraActor* camera;
	LightActor* light;
	Actor* dryBowser, *hammer;
	float tinkCounter;
public:
	explicit Scene0();
	virtual ~Scene0();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};