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

class Scene1 : public Scene, public Actor {
private:
	Ref<CameraActor> camera;
	Ref<LightActor> light;
	Ref<Actor> checkerBoard;
	std::vector<std::shared_ptr<Actor>> checkerRedList, checkerBlackList;
	int pickedID;
	int nextRow;
	float RowX, RowY;
public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};