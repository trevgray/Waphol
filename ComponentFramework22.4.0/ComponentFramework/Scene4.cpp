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

Scene4::Scene4(): RowX(0), RowY(0), nextRow(0), assetManager(nullptr) {
	Debug::Info("Created Scene4: ", __FILE__, __LINE__);
}

Scene4::~Scene4() {
	Debug::Info("Deleted Scene4: ", __FILE__, __LINE__);
	OnDestroy();  
}

bool Scene4::OnCreate() {
	Debug::Info("Loading assets Scene4: ", __FILE__, __LINE__);
	//assetManager
	assetManager = std::make_shared<AssetManager>();
	assetManager->BuildSceneAssets("OpenGLAssets.xml", "Scene4");
	assetManager->OnCreate();
	//camera
	AddActor<CameraActor>("camera", new CameraActor(nullptr));
	GetActor<CameraActor>()->InheritActor(assetManager->GetComponent<Actor>("camera"));
	GetActor<CameraActor>()->OnCreate();
	//light
	AddActor<LightActor>("light", new LightActor(nullptr));
	GetActor<LightActor>()->InheritActor(assetManager->GetComponent<Actor>("light"));
	GetActor<LightActor>()->OnCreate();
	//checkerboard
	AddActor<Actor>("checkerBoard", new Actor(nullptr));
	GetActor<Actor>("checkerBoard")->InheritActor(assetManager->GetComponent<Actor>("CheckerBoardActor"));
	GetActor<Actor>("checkerBoard")->OnCreate(); //The checkerboard is the 3rd Actor in the Scene

	//Red Checker creation loop
	RowX = RowY = nextRow = 0.0f;
	std::string checkerName;
	for (int x = 0; x <= 11; x++) {
		checkerName = "RedChecker" + std::to_string(x);;
		AddActor<Actor>(checkerName, new Actor(GetActor<Actor>("checkerBoard").get()));
		GetActor<Actor>(checkerName)->InheritActor(assetManager->GetComponent<Actor>("RedCheckerActor"));
		GetActor<Actor>(checkerName)->AddComponent<TransformComponent>(nullptr, Vec3(-4.5 + RowX, -4.3 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetActor<Actor>(checkerName)->OnCreate();
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
		AddActor<Actor>(checkerName, new Actor (GetActor<Actor>("checkerBoard").get()));
		GetActor<Actor>(checkerName)->InheritActor(assetManager->GetComponent<Actor>("BlackCheckerActor"));
		GetActor<Actor>(checkerName)->AddComponent<TransformComponent>(nullptr, Vec3(-3.225 + RowX, 4.4 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetActor<Actor>(checkerName)->OnCreate();
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
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E) {
			GetActor<Actor>("checkerBoard")->GetComponent<TransformComponent>()->SetTransform(GetActor<Actor>("checkerBoard")->GetComponent<TransformComponent>()->GetPosition(), GetActor<Actor>("checkerBoard")->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
			GetActor<Actor>("checkerBoard")->GetComponent<TransformComponent>()->SetTransform(GetActor<Actor>("checkerBoard")->GetComponent<TransformComponent>()->GetPosition(), GetActor<Actor>("checkerBoard")->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
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

void Scene4::Update(const float deltaTime) {
	
}

void Scene4::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<LightActor>()->GetLightID());

	glUseProgram(assetManager->GetComponent<ShaderComponent>("TextureShader")->GetProgram());

	for (auto actor : GetActorGraph()) {
		glUniformMatrix4fv(assetManager->GetComponent<ShaderComponent>("TextureShader")->GetUniformID("modelMatrix"), 1, GL_FALSE, actor.second->GetModelMatrix());
		if (actor.second->GetComponent<MaterialComponent>() != nullptr) { //everything is an actor, so i just check if it has a texture
			glBindTexture(GL_TEXTURE_2D, actor.second->GetComponent<MaterialComponent>()->getTextureID()); //this is also amazing because we can add as many actors as we want, and the render does not need to change
			actor.second->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}