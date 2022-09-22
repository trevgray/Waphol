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
#include "ControllerComponent.h"

Scene5::Scene5(): RowX(0), RowY(0), nextRow(0) {
	Debug::Info("Created Scene5: ", __FILE__, __LINE__);
}

Scene5::~Scene5() {
	Debug::Info("Deleted Scene5: ", __FILE__, __LINE__);
 
}

bool Scene5::OnCreate() {
	Debug::Info("Loading assets Scene5: ", __FILE__, __LINE__);
	EngineManager::Instance()->GetAssetManager()->LoadAssets("Assets.xml", "Scene4");
	EngineManager::Instance()->GetActorManager()->LoadNonPrehabActors();

	EngineManager::Instance()->GetActorManager()->GetActor<Actor>("CheckerBoardActor")->AddComponent<ControllerComponent>(nullptr, "PlayerController");
	EngineManager::Instance()->GetInputManager()->SetControllerActors(EngineManager::Instance()->GetActorManager()->GetActorGraph());

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

void Scene5::OnDestroy() {
	Debug::Info("Deleting assets Scene5: ", __FILE__, __LINE__);
}

void Scene5::HandleEvents(const SDL_Event &sdlEvent) {
	EngineManager::Instance()->GetInputManager()->HandleInputs(sdlEvent);
}

void Scene5::Update(const float deltaTime) {
	EngineManager::Instance()->GetActorManager()->UpdateActors(deltaTime);
}

void Scene5::Render() const {
	std::vector<std::string> shaders;
	shaders.push_back("TextureShader");
	EngineManager::Instance()->GetActorManager()->RenderActors(shaders);
}