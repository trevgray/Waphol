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
	explicit Scene0() {};
	virtual ~Scene0() {};

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& sdlEvent);

	// We'll use a ray to click on our geometry objects 
	// GEOMETRY::RayIntersectionInfo rayInfo; (TODO)
};

