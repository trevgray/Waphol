#pragma once
#include "Scene.h"
#include "AssetManager.h"

using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene3 : public Scene {
private:

public:
	explicit Scene3();
	virtual ~Scene3();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);

	virtual void UpdateGUI();
};