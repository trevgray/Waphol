#pragma once
#include "SceneManager.h"
#include "AssetManager.h"
#include "ActorManager.h"
#include "InputManager.h"
#include "NetworkManager.h"

#include "Timer.h"

#include <memory>
#include <thread>

class EngineManager {
private:
	//engine manager
	EngineManager();
	static Ref<EngineManager> instance;

	Ref<Timer> timer;

	unsigned int fps;
	bool isRunning;
	bool fullScreen;
	//game managers
	Ref<SceneManager> sceneManager;
	Ref<AssetManager> assetManager;
	Ref<ActorManager> actorManager;
	Ref<InputManager> inputManager;
	Ref<NetworkManager> networkManager;
	//threads
	//std::thread networkThread;
public:
	~EngineManager();

	bool Initialize();

	static Ref<EngineManager> Instance() {
		if (!instance) {
			instance = Ref<EngineManager>(new EngineManager);
		}
		return instance;
	}
	
	//Setters
	void SetIsRunning(bool isRunning_) { isRunning = isRunning_; }

	//Getters
	Ref<Timer> GetTimer() { return timer; }
	bool GetIsRunning() { return isRunning; }
	unsigned int GetFPS() { return fps; }
	unsigned int GetTimerSleepTime() { return timer->GetSleepTime(fps); }

	Ref<AssetManager> GetAssetManager() { return assetManager; }
	Ref<ActorManager> GetActorManager() { return actorManager; }
	Ref<InputManager> GetInputManager() { return inputManager; }
};