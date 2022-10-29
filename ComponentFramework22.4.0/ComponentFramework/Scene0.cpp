#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "ShapeComponent.h"

#include "Physics.h"

bool Scene0::OnCreate()
{
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene0");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	//std::vector<std::string> names{
	//	"CheckerBoardActor", "ActorChecker1", "ActorChecker2",
	//	"ActorDefaultChecker", "ActorTinyChecker", "ActorSkull",
	//	"ActorCube", "ActorCube2",
	//	"ActorMario"
	//};
	//for (const auto& name : names) {
	//	auto asset = assetManager->componentGraph.find(name);
	//	AddActor<Actor>(name, new Actor(nullptr));
	//	GetActor<Actor>(name)->InheritActor(assetManager->GetComponent<Actor>(name.c_str()));
	//	GetActor<Actor>(name)->OnCreate();
	//}

	//Vec3 rayStart = Vec3(2.0f, 0.0f, -2.0f);
	//Vec3 rayDirection = Vec3(0.0f, 1.0f, 0.0f);
	//GEOMETRY::Ray ray { rayStart, rayDirection };
	//GEOMETRY::Cylinder cylinder(10.0f, Vec3(0.0f, 5.0f, -2.0f), Vec3(1.0f, 9.0f, -2.0f));
	//Ref<ShapeComponent> shapeComponent = std::make_shared<ShapeComponent>(new Actor(nullptr), cylinder);
	//GEOMETRY::RayIntersectionInfo rayInfo = shapeComponent->shape->rayIntersectionInfo(ray);

	EngineManager::Instance()->GetActorManager()->AddActor<Actor>("Obstacle", new Actor(nullptr));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>("ObstacleActor"));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->AddComponent<TransformComponent>(nullptr, Vec3(-5.0f,0.0f,0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.15f, 0.15f, 0.15f));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->OnCreate();

	return true;
}

void Scene0::OnDestroy()
{
	
}


void Scene0::HandleEvents(const SDL_Event& sdlEvent)
{
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.0, 0.0, 0.0));
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.0, 0.0, 0.0));
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, 1.0));
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, -1.0));
			EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_M) {
			renderMeshes = !renderMeshes;
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_C) {
			renderCollisionShapes = !renderCollisionShapes;
		}

		break;
		
	case SDL_MOUSEBUTTONDOWN:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			GEOMETRY::Ray rayWorldSpace = EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->MakeWorldSpaceRayFromMouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
			
			/*std::cout << "START: " << rayWorldStart.x << " " << rayWorldStart.y << " " << rayWorldStart.z << std::endl;
			std::cout << "DIR: " << rayWorldDirection.x << " " << rayWorldDirection.y << " " << rayWorldDirection.z << std::endl;*/

			Hit hitResult = Physics::LineTrace(rayWorldSpace);

			if (hitResult.isIntersected) {
				std::cout << "You picked: " << hitResult.hitActorName << '\n';

				Vec3 actorPos = hitResult.hitActor->GetModelMatrix() * hitResult.intersectionPoint;

				//std::cout << rayInfo.intersectionPoint.x << " " << rayInfo.intersectionPoint.y << " " << rayInfo.intersectionPoint.z << std::endl;

				EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->GetComponent<TransformComponent>()->SetPosition(actorPos);

				//pickedActor = actor; // make a member variable called pickedActor. Will come in handy later...  
				//haveClickedOnSomething = true; // make this a member variable too. Set it to false before we loop over each actor
			}
		}
		break;

	default:
		break;
	}

}

void Scene0::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
}

void Scene0::Render() const
{
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
			if (renderMeshes) {
				actor.second->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
			}
			if (renderCollisionShapes) {
				if (actor.second->GetComponent<ShapeComponent>() != nullptr) {
					actor.second->GetComponent<ShapeComponent>()->Render();
				}
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

