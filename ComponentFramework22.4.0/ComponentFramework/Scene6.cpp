#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene6.h"

#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "SteeringComponent.h"
#include "FollowPath.h"
#include "ShapeComponent.h"
#include "Physics.h"
#include "ShaderComponent.h"

Scene6::Scene6() {
	Debug::Info("Created Scene6: ", __FILE__, __LINE__);
}

Scene6::~Scene6() {
	Debug::Info("Deleted Scene6: ", __FILE__, __LINE__);
}

bool Scene6::OnCreate() {
	Debug::Info("Loading assets Scene6: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene6");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors(); //Put load nav mesh in here (when i put it in XML)

	Vec3 actorPos[7] {Vec3(5.0f,-10.0f,-40.0f), Vec3(-5.0f,10.0f,-40.0f), Vec3(-18.0f,7.0f,-40.0f), Vec3(0.0f,-5.0f,-40.0f), Vec3(0.0f, 0.0f, -40.0f), Vec3(2.0f,7.0f,-40.0f), Vec3(18.0f,-10.0f,-40.0f) };

	for (int x = 0; x < 7; x++) {
		std::string obstacleName = "Obstacle" + std::to_string(x);
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>(obstacleName, new Actor(nullptr));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(obstacleName)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>("ObstacleActor"));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(obstacleName)->AddComponent<TransformComponent>(nullptr, actorPos[x], Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(obstacleName)->OnCreate();
	}

	EngineManager::Instance()->GetActorManager()->GetNavigationMesh()->OnCreate();
	EngineManager::Instance()->GetActorManager()->GetNavigationMesh()->DebugDraw();
	return true;
}

void Scene6::OnDestroy() {
	Debug::Info("Deleting assets Scene6: ", __FILE__, __LINE__);
}

void Scene6::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);

	if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
		GEOMETRY::Ray rayWorldSpace = EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->WorldSpaceRayFromMouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
		Hit hitResult = Physics::LineTrace(rayWorldSpace);
		if (hitResult.isIntersected) {
			std::cout << "You picked: " << hitResult.hitActorName << '\n';

			EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<SteeringComponent>()->GetSteeringBehaviour<FollowPath>()->SetGoal(stoi(hitResult.hitActorName));
		}
	}
}

void Scene6::UpdateGUI() {}

void Scene6::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
}

void Scene6::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}