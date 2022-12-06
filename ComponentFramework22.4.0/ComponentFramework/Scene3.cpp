#include <iostream>
#include <SDL.h>
#include "Scene3.h"

Scene3::Scene3() {
	Debug::Info("Created Scene3: ", __FILE__, __LINE__);
}

Scene3::~Scene3() {
	Debug::Info("Deleted Scene3: ", __FILE__, __LINE__);
}

bool Scene3::OnCreate() {
	Debug::Info("Loading assets Scene3: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene3");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	EngineManager::Instance()->GetNetworkManager()->SetAuthorityActor(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"));
	EngineManager::Instance()->GetNetworkManager()->AddReplicatedActor("0", EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"));
	return true;
}

void Scene3::OnDestroy() {
	Debug::Info("Deleting assets Scene3: ", __FILE__, __LINE__);
}

void Scene3::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
}

void Scene3::UpdateGUI() {}

void Scene3::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
}

void Scene3::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}