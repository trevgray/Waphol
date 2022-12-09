#include <iostream>
#include <SDL.h>
#include "Scene7.h"
#include "DecisionMakingComponent.h"

Scene7::Scene7() {
	Debug::Info("Created Scene7: ", __FILE__, __LINE__);
}

Scene7::~Scene7() {
	Debug::Info("Deleted Scene7: ", __FILE__, __LINE__);
}

bool Scene7::OnCreate() {
	Debug::Info("Loading assets Scene7: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene7");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	std::vector<std::string> xmls;
	xmls.push_back("DecisionMaking.xml");
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->AddComponent<DecisionMakingComponent>(nullptr, xmls);
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<DecisionMakingComponent>()->OnCreate();

	return true;
}

void Scene7::OnDestroy() {
	Debug::Info("Deleting assets Scene7: ", __FILE__, __LINE__);
}

void Scene7::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
}

void Scene7::UpdateGUI() {}

void Scene7::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
}

void Scene7::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}