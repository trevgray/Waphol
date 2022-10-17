#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene5.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"
#include "SteeringComponent.h"
#include "Seek.h"
#include "VelocityMatch.h"
#include "Arrive.h"
#include "Align.h"
#include "FaceVelocity.h"

Scene5::Scene5(): RowX(0), RowY(0), nextRow(0) {
	Debug::Info("Created Scene5: ", __FILE__, __LINE__);
}

Scene5::~Scene5() {
	Debug::Info("Deleted Scene5: ", __FILE__, __LINE__);
}

bool Scene5::OnCreate() {
	Debug::Info("Loading assets Scene5: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene5");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	EngineManager::Instance()->GetSoundManager()->AddSound("test", "media/getout.ogg", false, TwoD, 0.3f);
	EngineManager::Instance()->GetSoundManager()->PlaySound2D("test");

	return true;
}

void Scene5::OnDestroy() {
	Debug::Info("Deleting assets Scene5: ", __FILE__, __LINE__);
}

void Scene5::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
}

void Scene5::Update(const float deltaTime) {
	static float testTime = 0; 
	static bool testBool = true;
	testTime += deltaTime;
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);

	if (testTime > 10 && testBool == true) {
		std::cout << "test" << std::endl;
		testBool = false;
		//EngineManager::Instance()->GetSoundManager()->DeleteSound("test");
		EngineManager::Instance()->GetSoundManager()->FadeOutSound("test", 0.0f, 0.0001f);
	}

	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<PhysicsBodyComponent>()->GetAccel().z << std::endl;
	//std::cout << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().x << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().y << " " << EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")->GetComponent<TransformComponent>()->GetPosition().z << std::endl;
}

void Scene5::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}