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
	EngineManager::Instance()->GetActorManager()->SetNavigationMesh(std::make_shared<NavigationMesh>());
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

	std::vector<std::string> test;
	test.push_back("Player");
	test.push_back("NPC");
	EngineManager::Instance()->GetActorManager()->GetNavigationMesh()->Initialize(Vec3(-28.0f, -15.0f, 0.0f), Vec3(28.0f, 15.0f, 0.0f), test);

	GEOMETRY::Sphere nodeSphere(Vec3(), 1.0f);

	for (auto node : EngineManager::Instance()->GetActorManager()->GetNavigationMesh()->GetVoronoiGraph().GetNodes()) {
		std::string nodeName = std::to_string(node.second.GetLabel());
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>(nodeName, new Actor(nullptr));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->DeleteParent();
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<TransformComponent>(nullptr, node.second.GetPos(), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<MeshComponent>(nullptr, "meshes/Sphere.obj");
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<MaterialComponent>(nullptr, "textures/surface.jpg");
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<ShapeComponent>(nullptr, nodeSphere);
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->OnCreate();
	}

	/*std::vector<Ref<SteeringBehaviour>> npcSteering;
	npcSteering.push_back(std::make_shared<FollowPath>(0.5f, 1.5f, 0.1f));

	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->AddComponent<SteeringComponent>(nullptr, npcSteering);
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<SteeringComponent>()->OnCreate();*/

	//dynamic_cast<FollowPath*>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<SteeringComponent>()->GetSteeringBehaviour<FollowPath>().get())->SetNavMesh(navMesh);
	//dynamic_cast<FollowPath*>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<SteeringComponent>()->GetSteeringBehaviour<FollowPath>().get())->SetGoal(9);

	return true;
}

void Scene6::OnDestroy() {
	Debug::Info("Deleting assets Scene5: ", __FILE__, __LINE__);
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

	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().z << std::endl;
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().z << std::endl;
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player")->GetComponent<TransformComponent>()->GetPosition().z << std::endl;
}

void Scene6::Render() const {
	/*std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);*/

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 0.6f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, EngineManager::Instance()->GetActorManager()->GetActor<LightActor>()->GetLightID());

	glUseProgram(EngineManager::Instance()->GetAssetManager()->GetComponent<ShaderComponent>("TextureShader")->GetProgram());

	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		glUniformMatrix4fv(EngineManager::Instance()->GetAssetManager()->GetComponent<ShaderComponent>("TextureShader")->GetUniformID("modelMatrix"), 1, GL_FALSE, actor.second->GetModelMatrix());
		if (actor.second->GetComponent<MaterialComponent>() != nullptr) {
			glBindTexture(GL_TEXTURE_2D, actor.second->GetComponent<MaterialComponent>()->getTextureID());
				actor.second->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
				if (actor.second->GetComponent<ShapeComponent>() != nullptr) {
					actor.second->GetComponent<ShapeComponent>()->Render();
				}
			}
		}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}