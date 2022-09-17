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

bool Scene0::OnCreate()
{
	LoadAssetManager("OpenGLAssets.xml", "Scene0");
	LoadNonPrehabActors();

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
			GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.0, 0.0, 0.0));
			GetActor<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.0, 0.0, 0.0));
			GetActor<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, 1.0));
			GetActor<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetActor<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, -1.0));
			GetActor<CameraActor>()->UpdateViewMatrix();
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

			// Loop through all the actors and check if the ray has collided with them
			// Pick the one with the smallest positive t value
			/*for (auto it = actors.begin(); it != actors.end(); ++it) {
				Ref<Actor> actor = std::dynamic_pointer_cast<Actor>(it->second);
				Ref<TransformComponent> transformComponent = actor->GetComponent <TransformComponent>();
				Ref<ShapeComponent> shapeComponent = actor->GetComponent <ShapeComponent>();
				if (shapeComponent->shapeType == ShapeType::sphere
					//|| shapeComponent->shapeType == ShapeType::cylinder (TODO)
					//|| shapeComponent->shapeType == ShapeType::capsule (TODO)
					//|| shapeComponent->shapeType == ShapeType::box (TODO)
					) {
					// TODO: Transform the ray into the local space of the object and check if a collision occurred
				}
			}*/
		}
		break;

	default:
		break;
	}

}

void Scene0::Update(const float deltaTime) {
}

void Scene0::Render() const
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<LightActor>()->GetLightID());

	glUseProgram(GetAssetManager()->GetComponent<ShaderComponent>("TextureShader")->GetProgram());

	for (auto actor : GetActorGraph()) {
		glUniformMatrix4fv(GetAssetManager()->GetComponent<ShaderComponent>("TextureShader")->GetUniformID("modelMatrix"), 1, GL_FALSE, actor.second->GetModelMatrix());
		if (actor.second->GetComponent<MaterialComponent>() != nullptr) { //everything is an actor, so i just check if it has a texture
			glBindTexture(GL_TEXTURE_2D, actor.second->GetComponent<MaterialComponent>()->getTextureID()); //this is also amazing because we can add as many actors as we want, and the render does not need to change
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

