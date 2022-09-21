#pragma once
#include "SceneManager.h"
#include "AssetManager.h"
#include "ActorManager.h"
#include "InputManager.h"
#include <memory>

class EngineManager {
private:
	EngineManager();
	static Ref<EngineManager> instance;

	Ref<SceneManager> sceneManager;
	Ref<AssetManager> assetManager;
	Ref<ActorManager> actorManager;
	Ref<InputManager> inputManager;
public:
	~EngineManager();

	bool Initialize();

	static Ref<EngineManager> Instance() {
		if (!instance) {
			instance = Ref<EngineManager>(new EngineManager);
		}
		return instance;
	}

	Ref<AssetManager> GetAssetManager() { return assetManager; }
	Ref<ActorManager> GetActorManager() { return actorManager; }
	Ref<InputManager> GetInputManager() { return inputManager; }
};