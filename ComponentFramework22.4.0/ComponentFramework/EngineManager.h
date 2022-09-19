#pragma once
#include "SceneManager.h"
#include "AssetManager.h"
#include <memory>
#include <string>

class EngineManager {
private:
	std::unique_ptr<SceneManager> sceneManager;
	AssetManager* assetManager;
public:
	EngineManager();
	~EngineManager();

	void Initialize();
};