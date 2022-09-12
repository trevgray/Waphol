#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene0.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"

Scene0::Scene0(): camera(nullptr), dryBowser(nullptr), light(nullptr) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
	tinkCounter = 0.0f;
}

Scene0::~Scene0() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
	
}

bool Scene0::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	camera = new CameraActor(nullptr);
	camera->AddComponent<TransformComponent>(nullptr,Vec3(0.0f,0.0f,-5.0f), Quaternion());
	camera->OnCreate();

	light = new LightActor(nullptr, LightStyle::DirectionLight, Vec3(0.0f,0.0f,0.0f), Vec4(0.8f,0.8f,0.8f,0.0f));
	light->OnCreate();

	dryBowser = new Actor(nullptr);
	dryBowser->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, -1.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.1f,0.1f,0.1f));
	dryBowser->AddComponent<MeshComponent>(nullptr, "meshes/dryBowser.obj");
	dryBowser->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	dryBowser->AddComponent<MaterialComponent>(nullptr, "textures/DryKoopaAll.png");
	dryBowser->OnCreate();

	hammer = new Actor(dryBowser);
	hammer->AddComponent<TransformComponent>(nullptr, Vec3(10.0f, 4.0f, 0.0f), Quaternion(0.50f, 0.50f, 0.71f, 0.0f), Vec3(10.0f, 10.0f, 10.0f));
	hammer->AddComponent<MeshComponent>(nullptr, "meshes/Hammer.obj");
	hammer->AddComponent<MaterialComponent>(nullptr, "textures/hammer_BaseColor.png");
	hammer->OnCreate();

	return true;
}

void Scene0::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	if (camera) delete camera;
	if (dryBowser) delete dryBowser;
	if (light) delete light;
}

void Scene0::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
		//if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
		//	camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.0, 0.0, 0.0));
		//	camera->UpdateViewMatrix();
		//}
		//else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
		//	camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.0, 0.0, 0.0));
		//	camera->UpdateViewMatrix();
		//}
		//else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
		//	camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, 1.0));
		//	camera->UpdateViewMatrix();
		//}
		//else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
		//	camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, -1.0));
		//	camera->UpdateViewMatrix();
		//}
		//else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E) {
		//	dryBowser->GetComponent<TransformComponent>()->SetTransform(dryBowser->GetComponent<TransformComponent>()->GetPosition(), dryBowser->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)), Vec3(0.1f, 0.1f, 0.1f));
		//}
		//else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
		//	dryBowser->GetComponent<TransformComponent>()->SetTransform(dryBowser->GetComponent<TransformComponent>()->GetPosition(), dryBowser->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)), Vec3(0.1f, 0.1f, 0.1f));
		//}
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

void Scene0::Update(const float deltaTime) {
	//tinkCounter += deltaTime; //used for angle - because sin(365 degrees) == sin(5 degrees) 
	//dryBowser->GetComponent<TransformComponent>()->setRotation(QMath::angleAxisRotation(tinkCounter * 50, Vec3(0, 1, 0))); //used https://eater.net/quaternions to visualize Quaternions
}

void Scene0::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Ref<ShaderComponent> shader = dryBowser->GetComponent<ShaderComponent>();
	Ref<MeshComponent> mesh = dryBowser->GetComponent<MeshComponent>();
	Ref <MaterialComponent> texture = dryBowser->GetComponent<MaterialComponent>();
	if (shader == nullptr || mesh == nullptr) {
		return;
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, dryBowser->GetModelMatrix());
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);

	mesh = hammer->GetComponent<MeshComponent>();
	texture = hammer->GetComponent<MaterialComponent>();
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, hammer->GetModelMatrix());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}