#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"
#include "ShapeComponent.h"

#include "Physics.h"

#include "PMath.h"
#include "PhysicsBodyComponent.h"

bool Scene1::OnCreate()
{
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene1");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second->GetComponent<TransformComponent>() != nullptr) {
			actor.second->AddComponent<PhysicsBodyComponent>(nullptr, actor.second->GetComponent<TransformComponent>(), 10.0f, Vec3(), Vec3(), 999.0f, 999.0f, Matrix3(), Vec3(), Vec3(), 999.0f);
		}
	}

	EngineManager::Instance()->GetActorManager()->AddActor<Actor>("Obstacle", new Actor(nullptr));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>("ObstacleActor"));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->AddComponent<TransformComponent>(nullptr, Vec3(-5.0f,0.0f,0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.15f, 0.15f, 0.15f));
	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->OnCreate();

	return true;
}

void Scene1::OnDestroy()
{
	
}


void Scene1::HandleEvents(const SDL_Event& sdlEvent)
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
			GEOMETRY::Ray rayWorldSpace = EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->WorldSpaceRayFromMouseCoords(static_cast<float>(sdlEvent.button.x), static_cast<float>(sdlEvent.button.y));
			
			/*std::cout << "START: " << rayWorldStart.x << " " << rayWorldStart.y << " " << rayWorldStart.z << std::endl;
			std::cout << "DIR: " << rayWorldDirection.x << " " << rayWorldDirection.y << " " << rayWorldDirection.z << std::endl;*/

			Hit hitResult = Physics::LineTrace(rayWorldSpace);

			if (hitResult.isIntersected) {
				std::cout << "You picked: " << hitResult.hitActorName << '\n';

				Vec3 actorPos = hitResult.hitActor->GetModelMatrix() * hitResult.intersectionPoint;
				intersectionPoint = hitResult.hitActor->GetModelMatrix() * hitResult.intersectionPoint;

				//std::cout << rayInfo.intersectionPoint.x << " " << rayInfo.intersectionPoint.y << " " << rayInfo.intersectionPoint.z << std::endl;

				EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Obstacle")->GetComponent<TransformComponent>()->SetPosition(actorPos);

				pickedActor = hitResult.hitActor; // make a member variable called pickedActor. Will come in handy later...  
				haveClickedOnSomething = true; // make this a member variable too. Set it to false before we loop over each actor
			}
		}
		break;

	default:
		break;
	}

}

void Scene1::UpdateGUI() {
	{
		static float rotation = 0.0f;
		static Vec3 position = Vec3(0.0f, 0.0f, 0.0f);

		ImGui::Begin("Camera Controls"); // Create a window and append into it.
		ImGui::Text("This controls the camera."); // Display some text (you can use a format strings too)
		ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f); // a slider from 0.0f to 1.0f
		EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->setOrientation(QMath::angleAxisRotation(rotation, Vec3(0.0f, 1.0f, 0.0f)));
		ImGui::Text("Position: %f %f %f", position.x, position.y, position.z);
		if (ImGui::Button("X+")) { //Buttons return true when clicked (most widgets return true when edited/activated)
			position.x = position.x + 1.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Y+")) {
			position.y = position.y + 0.5f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Z+")) {
			position.z = position.z + 1.0f;
		}

		if (ImGui::Button("X-")) { //Buttons return true when clicked (most widgets return true when edited/activated)
			position.x = position.x - 1.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Y-")) {
			position.y = position.y - 0.5f;
		}
		ImGui::SameLine();
		if (ImGui::Button("Z-")) {
			position.z = position.z - 1.0f;
		}

		//EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(position);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->UpdateViewMatrix();
}

void Scene1::Update(const float deltaTime) {
	//EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
	if (haveClickedOnSomething) {
		Ref<PhysicsBodyComponent> physicsBody = pickedActor->GetComponent<PhysicsBodyComponent>();
		Ref<TransformComponent> transform = pickedActor->GetComponent<TransformComponent>();
		float dragCoeff = 0.2f;
		Vec3 dragForce = physicsBody->GetVel() * (-dragCoeff);
		Vec3 gravityForce(0.0f, -9.81f * physicsBody->GetMass(), 0.0f);
		Vec3 netForce = gravityForce + dragForce;
		Physics::ApplyForce(pickedActor, netForce);
		//calculate a fist approximation of velocity based on acceleration
		physicsBody->SetVel(physicsBody->GetVel() + physicsBody->GetAccel() * deltaTime);
		//use constraint to correct velocity errors
		Physics::MouseConstraint(pickedActor, deltaTime, intersectionPoint);
		//update position using corrected velocities
		transform->SetPosition(transform->GetPosition() + physicsBody->GetVel() * deltaTime);
		//we can rotate too with the mouse constraint - so update orientation too
		Quaternion angularVelQuaternion(0.0f, physicsBody->GetAngularVel());
		//Rotate using q = q + 0.5twq
		transform->setOrientation(transform->GetQuaternion() + angularVelQuaternion * transform->GetQuaternion() * 0.5f * deltaTime);
		//don't forget to normalize after too - Only unit quaternions please - Otherwise the model stretches
		transform->setOrientation(QMath::normalize(transform->GetQuaternion()));
	}
}

void Scene1::Render() const
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

