#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene4.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

Scene4::Scene4(): RowX(0), RowY(0), nextRow(0) {
	Debug::Info("Created Scene4: ", __FILE__, __LINE__);
}

Scene4::~Scene4() {
	Debug::Info("Deleted Scene4: ", __FILE__, __LINE__);
 
}

bool Scene4::OnCreate() {
	Debug::Info("Loading assets Scene4: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene4");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	//Red Checker creation loop
	RowX = RowY = nextRow = 0.0f;
	std::string checkerName;
	for (int x = 0; x <= 11; x++) {
		checkerName = "RedChecker" + std::to_string(x);;
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>(checkerName, new Actor(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor").get()));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(checkerName)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>("RedCheckerActor"));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(checkerName)->AddComponent<TransformComponent>(nullptr, Vec3(-4.5f + RowX, -4.3f + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(checkerName)->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0.0f;
			RowY += 1.26f;
			if (RowY == 1.26f) {
				RowX = 1.27f;
			}
		}
	}
	//Black Checker creation loop
	RowX = RowY = nextRow = 0.0f;
	for (int x = 0; x <= 11; x++) {
		checkerName = "BlackChecker" + std::to_string(x);
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>(checkerName, new Actor (EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor").get()));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(checkerName)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>("BlackCheckerActor"));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(checkerName)->AddComponent<TransformComponent>(nullptr, Vec3(-3.225f + RowX, 4.4f + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(checkerName)->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0.0f;
			RowY -= 1.26f;
			if (RowY == -1.26f) {
				RowX = -1.27f;
			}
		}
	}
	return true;
}

void Scene4::OnDestroy() {
	Debug::Info("Deleting assets Scene4: ", __FILE__, __LINE__);
}

void Scene4::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
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
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E) {
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor")->GetComponent<TransformComponent>()->SetTransform(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor")->GetComponent<TransformComponent>()->GetPosition(), EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor")->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor")->GetComponent<TransformComponent>()->SetTransform(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor")->GetComponent<TransformComponent>()->GetPosition(), EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor")->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		break;

	case SDL_MOUSEMOTION:          
		break;

	case SDL_MOUSEBUTTONDOWN:              
		break; 

	case SDL_MOUSEBUTTONUP:            
	break;

	default:
		break;
    }
}

void Scene4::UpdateGUI() {
	/*bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);*/

	{
		static float rotation = 0.0f;
		static Vec3 position = Vec3(0.0f, 0.0f, -13.0f);

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

		EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(position);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->UpdateViewMatrix();
}

void Scene4::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
}

void Scene4::Render() const {
	/*glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, EngineManager::Instance()->GetActorManager()->GetActor<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, EngineManager::Instance()->GetActorManager()->GetActor<LightActor>()->GetLightID());

	glUseProgram(EngineManager::Instance()->GetAssetManager()->GetComponent<ShaderComponent>("TextureShader")->GetProgram());

	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		glUniformMatrix4fv(EngineManager::Instance()->GetAssetManager()->GetComponent<ShaderComponent>("TextureShader")->GetUniformID("modelMatrix"), 1, GL_FALSE, actor.second->GetModelMatrix());
		if (actor.second->GetComponent<MaterialComponent>() != nullptr) { //everything is an actor, so i just check if it has a texture
			glBindTexture(GL_TEXTURE_2D, actor.second->GetComponent<MaterialComponent>()->getTextureID()); //this is also amazing because we can add as many actors as we want, and the render does not need to change
			actor.second->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);*/

	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}