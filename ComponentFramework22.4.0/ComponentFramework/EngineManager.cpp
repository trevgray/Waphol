#include "EngineManager.h"

EngineManager::EngineManager() {
	sceneManager = std::make_unique<SceneManager>();
	assetManager = new AssetManager();
}
EngineManager::~EngineManager() {

}

void EngineManager::Initialize() {
	if (sceneManager->Initialize("Waphol Engine", 1280, 720) == true) {
		sceneManager->Run();
	}
}