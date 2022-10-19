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
#include "Ray.h"

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
			Vec3 mouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y), 0.0f);
			// TODO: get a ray pointing into the world
			// We have the x, y pixel coordinates
			// Need to convert this into world space to build our ray
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			Matrix4 ndc = MMath::viewportNDC(viewport[2], viewport[3]);
			Matrix4 projection = EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetProjectionMatrix();
			Matrix4 view = EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetViewMatrix();
			Matrix4 rayTransform = MMath::inverse(ndc * projection * view);

			Vec3 rayWorldStart = Vec3();
			Vec3 rayWorldDirection = VMath::normalize(rayTransform * mouseCoords);

			GEOMETRY::Ray ray{ rayWorldStart, rayWorldDirection };

			std::cout << "START: " << rayWorldStart.x << " " << rayWorldStart.y << " " << rayWorldStart.z << std::endl;
			std::cout << "DIR: " << rayWorldDirection.x << " " << rayWorldDirection.y << " " << rayWorldDirection.z << std::endl;

			// Loop through all the actors and check if the ray has collided with them
			// Pick the one with the smallest positive t value

			EngineManager::Instance()->GetActorManager()->GetActorGraph().begin();

			for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
				glUniformMatrix4fv(EngineManager::Instance()->GetAssetManager()->GetComponent<ShaderComponent>("TextureShader")->GetUniformID("modelMatrix"), 1, GL_FALSE, actor.second->GetModelMatrix());
				if (actor.second->GetComponent<ShapeComponent>() != nullptr) {
					Ref<TransformComponent> transformComponent = actor.second->GetComponent<TransformComponent>();
					Ref<ShapeComponent> shapeComponent = actor.second->GetComponent<ShapeComponent>();
					if (shapeComponent->shapeType == ShapeType::sphere
						//|| shapeComponent->shapeType == ShapeType::cylinder (TODO)
						//|| shapeComponent->shapeType == ShapeType::capsule (TODO)
						//|| shapeComponent->shapeType == ShapeType::box (TODO)
						) {
						//Transform the ray into the local space of the object and check if a collision occurred
						Vec3 rayStartInObjectSpace = MMath::inverse(actor.second->GetModelMatrix()) * ray.start;
						Vec3 rayDirInObjectSpace = MMath::inverse(actor.second->GetModelMatrix()).multiplyWithoutDividingOutW(Vec4(ray.dir, 0.0f));

						GEOMETRY::Ray rayInObjectSpace{ rayStartInObjectSpace, rayDirInObjectSpace }; 
						GEOMETRY::RayIntersectionInfo rayInfo = shapeComponent->shape->rayIntersectionInfo(rayInObjectSpace);

						if (rayInfo.isIntersected) {
							std::cout << "You picked: " << actor.first << '\ n';
							//pickedActor = actor; // make a member variable called pickedActor. Will come in handy later...  
							//haveClickedOnSomething = true; // make this a member variable too. Set it to false before we loop over each actor
						} 
					}
				}
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
				actor.second->GetComponent<ShapeComponent>()->Render();
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

