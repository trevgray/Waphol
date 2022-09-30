#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene5.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"
#include "ControllerComponent.h"
#include "PhysicsBodyComponent.h"
#include "SteeringComponent.h"
#include "Seek.h"
#include "VelocityMatch.h"
#include "Arrive.h"
#include "Align.h"

Scene5::Scene5(): RowX(0), RowY(0), nextRow(0) {
	Debug::Info("Created Scene5: ", __FILE__, __LINE__);
}

Scene5::~Scene5() {
	Debug::Info("Deleted Scene5: ", __FILE__, __LINE__);
}

bool Scene5::OnCreate() {
	Debug::Info("Loading assets Scene5: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene5");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->AddComponent<PhysicsBodyComponent>(nullptr, EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>(), Vec3(), Vec3(), 1.0f, 1.0f, 0.0f, 0.0f, 15.0f, 20.0f, 30.0f, 10.0f);
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->AddComponent<ControllerComponent>(nullptr, "PlayerController");
	EngineManager::Instance()->GetInputManager()->SetControllerActors(EngineManager::Instance()->GetActorManager()->GetActorGraph());

	std::vector<Ref<SteeringBehaviour>> steeringBehaviours;
	//steeringBehaviours.push_back(std::make_shared<Arrive>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()));
	//steeringBehaviours.push_back(std::make_shared<Seek>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()));
	//steeringBehaviours.push_back(std::make_shared<VelocityMatch>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 0.1));
	steeringBehaviours.push_back(std::make_shared<Align>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>(), EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetQuaternion(), 1.0f, 10.0f, 0.1f));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->AddComponent<SteeringComponent>(nullptr, steeringBehaviours, EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"));

	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->AddComponent<PhysicsBodyComponent>(nullptr, EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>());
	return true;
}

void Scene5::OnDestroy() {
	Debug::Info("Deleting assets Scene5: ", __FILE__, __LINE__);
}

void Scene5::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
}

void Scene5::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().z << std::endl;
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().z << std::endl;
}

void Scene5::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}