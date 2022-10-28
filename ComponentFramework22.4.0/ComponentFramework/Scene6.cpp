#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Scene6.h"

#include "MeshComponent.h"
#include "MaterialComponent.h"

Scene6::Scene6() {
	Debug::Info("Created Scene6: ", __FILE__, __LINE__);
	navMesh = std::make_shared<NavigationMesh>();
}

Scene6::~Scene6() {
	Debug::Info("Deleted Scene6: ", __FILE__, __LINE__);
}

bool Scene6::OnCreate() {
	Debug::Info("Loading assets Scene6: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene6");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	Vec3 actorPos[5] {Vec3(-5.0f,-13.0f,0.0f), Vec3(-5.0f,10.0f,0.0f), Vec3(-18.0f,7.0f,0.0f), Vec3(15.0f,-7.0f,0.0f), Vec3(0.0f, 0.0f, 0.0f) };



	for (int x = 0; x < 5; x++) {
		std::string obstacleName = "Obstacle" + std::to_string(x);
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>(obstacleName, new Actor(nullptr));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(obstacleName)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>("ObstacleActor"));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(obstacleName)->AddComponent<TransformComponent>(nullptr, actorPos[x], Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(obstacleName)->OnCreate();
	}

	std::vector<std::string> test;
	test.push_back("Player");
	test.push_back("NPC");
	navMesh->Initialize(Vec3(-28.0f,-15.0f,0.0f), Vec3(28.0f, 15.0f, 0.0f), test);

	for (auto node : navMesh->GetVoronoiGraph().GetNodes()) {
		std::string nodeName = std::to_string(node.second.GetLabel());
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>(nodeName, new Actor(nullptr));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<TransformComponent>(nullptr, node.second.GetPos(), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<MeshComponent>(nullptr, "meshes/Cube.obj");
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png");
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->OnCreate();
	}

	/*std::vector<int> path;
	int current = 0;

	int goal = 10;

	std::vector<int> cameFrom = navMesh->GetVoronoiGraph().Dijkstra(goal, 0);
	while (current != goal) {
		path.push_back(current);
		current = cameFrom[current];
	}

	for (int loop : path) {
		std::cout << loop << std::endl;
	}*/

	return true;
}

void Scene6::OnDestroy() {
	Debug::Info("Deleting assets Scene5: ", __FILE__, __LINE__);
}

void Scene6::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
}

void Scene6::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);

	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().z << std::endl;
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().z << std::endl;
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition().z << std::endl;
}

void Scene6::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}