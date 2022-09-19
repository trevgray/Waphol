#include "EngineManager.h"

EngineManager::EngineManager() {
	sceneManager = std::make_unique<SceneManager>();
	assetManager = std::make_unique<AssetManager>();
}

EngineManager::~EngineManager() {
	Debug::Info("Deleting EngineManager: ", __FILE__, __LINE__);
}

bool EngineManager::Initialize() {
	if (sceneManager->Initialize("Game Engine", 1280, 720) == true) {
		sceneManager->Run();
		return true;
	}
	return false;
}

Ref<EngineManager> EngineManager::instance = 0;