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

class Scene3 : public Scene {
private:
	int nextRow;
	float RowX, RowY;
	Ref<AssetManager> assetManager;
	//maybe the assetManager should also be inherited for Scene.h - that would also fix the scope issue
public:
	explicit Scene3();
	virtual ~Scene3();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};