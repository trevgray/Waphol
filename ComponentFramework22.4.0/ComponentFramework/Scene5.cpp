#include <iostream>
#include <SDL.h>
#include "Scene5.h"

Scene5::Scene5() {
	Debug::Info("Created Scene5: ", __FILE__, __LINE__);
}

Scene5::~Scene5() {
	Debug::Info("Deleted Scene5: ", __FILE__, __LINE__);
}

bool Scene5::OnCreate() {
	Debug::Info("Loading assets Scene5: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene5");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	return true;
}

void Scene5::OnDestroy() {
	Debug::Info("Deleting assets Scene5: ", __FILE__, __LINE__);
}

void Scene5::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
}

void Scene5::UpdateGUI() {
	//EngineManager::Instance()->GetProfilerManager()->RenderProfiler();
}

void Scene5::Update(const float deltaTime) {
	//EngineManager::Instance()->GetProfilerManager()->ProfileScope("Update");
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);

	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().z << std::endl;
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().z << std::endl;
}

void Scene5::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}