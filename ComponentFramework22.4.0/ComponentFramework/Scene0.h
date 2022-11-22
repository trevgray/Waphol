#pragma once
#include "Scene.h"
#include <unordered_map>
#include "Actor.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "AssetManager.h"

struct Scene0 : public Scene {
	bool renderMeshes = true;
	bool renderCollisionShapes = true;

	Ref<Actor> pickedActor;
	Vec3 intersectionPoint;
	bool haveClickedOnSomething = false;

	explicit Scene0() {};
	virtual ~Scene0() {};

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	virtual void UpdateGUI();
	// We'll use a ray to click on our geometry objects 
	// GEOMETRY::RayIntersectionInfo rayInfo; (TODO)
};

