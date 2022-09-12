#pragma once
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "Actor.h"
#include "AssetManager.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene2 : public Scene {
private:
	int nextRow;
	float RowX, RowY;
	AssetManager* assetManager; // i feel like the assetManager should not be a raw pointer, but idk how to make smart pointers in the scope of the entire scene - maybe putting it in the sceneManager?
	//maybe the assetManager should also be inherited for Scene.h - that would also fix the scope issue
public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};