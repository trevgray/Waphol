#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1v2.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"

Scene1v2::Scene1v2(): RowX(0), RowY(0), nextRow(0) {
	Debug::Info("Created Scene1v2: ", __FILE__, __LINE__);
}

Scene1v2::~Scene1v2() {
	Debug::Info("Deleted Scene1v2: ", __FILE__, __LINE__);
	
}

bool Scene1v2::OnCreate() {
	Debug::Info("Loading assets Scene1v2: ", __FILE__, __LINE__);
	//camera
	AddActor<CameraActor>("camera", new CameraActor(nullptr));
	GetActor<CameraActor>()->AddComponent<TransformComponent>(nullptr,Vec3(0.0f,0.0f,-12.0f), Quaternion());
	GetActor<CameraActor>()->OnCreate();
	//light
	AddActor<LightActor>("light", new LightActor(nullptr, LightStyle::DirectionLight, Vec3(0.0f, 10.0f, 0.0f), Vec4(0.8f, 0.8f, 0.8f, 0.0f)));
	GetActor<LightActor>()->OnCreate();
	//checkerboard
	AddActor<Actor>("checkerBoard", new Actor(nullptr));
	GetActor<Actor>("checkerBoard")->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f,1.0f,1.0f));
	GetActor<Actor>("checkerBoard")->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	GetActor<Actor>("checkerBoard")->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	GetActor<Actor>("checkerBoard")->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	GetActor<Actor>("checkerBoard")->OnCreate(); //The checkerboard is the 3rd Actor in the Scene

	//Red Checker creation loop
	//std::string checkerString = (Red Checker);
	RowX = RowY = nextRow = 0.0f;
	for (int x = 3; x <= 14; x++) {
		AddActor<Actor>("Red Checker" + x, new Actor(GetActor<Actor>("checkerBoard").get()));
		GetActor<Actor>("Red Checker" + x)->AddComponent<TransformComponent>(nullptr, Vec3(-4.5 + RowX, -4.3 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetActor<Actor>("Red Checker" + x)->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		GetActor<Actor>("Red Checker" + x)->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png"); //think about removing these
		GetActor<Actor>("Red Checker" + x)->OnCreate();
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
	for (int x = 15; x <= 27; x++) {
		AddActor<Actor>("Black Checker" + x, new Actor(GetActor<Actor>("checkerBoard").get()));
		GetActor<Actor>("Black Checker" + x)->AddComponent<TransformComponent>(nullptr, Vec3(-3.225 + RowX, 4.4 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetActor<Actor>("Black Checker" + x)->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		GetActor<Actor>("Black Checker" + x)->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png"); //think about removing these
		GetActor<Actor>("Black Checker" + x)->OnCreate();
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

void Scene1v2::OnDestroy() {
	Debug::Info("Deleting assets Scene1v2: ", __FILE__, __LINE__);
}

void Scene1v2::HandleEvents(const SDL_Event &sdlEvent) {
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

void Scene1v2::Update(const float deltaTime) {
	
}

void Scene1v2::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUseProgram(GetComponent<Actor>(2)->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<LightActor>()->GetLightID());
	//this uses the old method - switch the scene2's render
	//for (int x = 2; x <= GetComponentVectorSize() - 2; x++) { //-2 because the first 2 components are the camera and light actor - a smarter system is probably better like checking if the component is an actor
	//	glUniformMatrix4fv(GetComponent<Actor>(2)->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, GetComponent<Actor>(x)->GetModelMatrix());
	//	glBindTexture(GL_TEXTURE_2D, GetComponent<Actor>(x)->GetComponent<MaterialComponent>()->getTextureID());
	//	GetComponent<Actor>(x)->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	//}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}