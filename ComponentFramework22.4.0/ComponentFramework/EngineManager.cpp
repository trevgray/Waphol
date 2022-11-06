#include "EngineManager.h"

EngineManager::EngineManager() : fps(60), isRunning(false), fullScreen(false) {
	timer = std::make_unique<Timer>();
	if (timer == nullptr) {
		Debug::FatalError("Failed to initialize Timer object", __FILE__, __LINE__);
		return;
	}
	//managers
	sceneManager = std::make_unique<SceneManager>();
	assetManager = std::make_unique<AssetManager>();
	actorManager = std::make_unique<ActorManager>();
	inputManager = std::make_unique<InputManager>();
	networkManager = std::make_unique<NetworkManager>();
	soundManager = std::make_unique<SoundManager>();
}

EngineManager::~EngineManager() {
	Debug::Info("Deleting EngineManager: ", __FILE__, __LINE__);
}

bool EngineManager::Initialize() {
	//open the engine xml here
	networkManager->Initialize(Server);

	//start the game loop
	timer->Start();
	isRunning = true;
	if (sceneManager->Initialize("Game Engine", 1280, 720) == false) {
		return false;
	}
	std::thread networkThread(&NetworkManager::Run, networkManager);
	networkThread.detach();
	while (isRunning) {
		timer->UpdateFrameTicks();
		sceneManager->Run();
	}
	return true;
}

Ref<EngineManager> EngineManager::instance = 0;