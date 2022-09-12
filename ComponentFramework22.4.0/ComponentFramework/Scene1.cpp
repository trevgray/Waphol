#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"

Scene1::Scene1(): Actor(nullptr), camera(nullptr), checkerBoard(nullptr), light(nullptr), RowX(0), RowY(0), nextRow(0), pickedID(0) {
	Debug::Info("Created Scene1: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {
	Debug::Info("Deleted Scene1: ", __FILE__, __LINE__);
}

bool Scene1::OnCreate() {
	Debug::Info("Loading assets Scene1: ", __FILE__, __LINE__);
	camera = std::make_shared<CameraActor>(nullptr);
	camera->AddComponent<TransformComponent>(nullptr,Vec3(0.0f,0.0f,-12.0f), Quaternion());
	camera->OnCreate();
	light = std::make_shared<LightActor>(nullptr, LightStyle::DirectionLight, Vec3(0.0f, 10.0f, 0.0f), Vec4(0.8f, 0.8f, 0.8f, 0.0f));
	light->OnCreate();
	checkerBoard = std::make_shared<Actor>(nullptr);
	checkerBoard->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f,1.0f,1.0f));
	checkerBoard->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	//checkerBoard->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl"); //for the texture shader
	checkerBoard->AddComponent<ShaderComponent>(nullptr, "shaders/colourPickerVert.glsl", "shaders/colourPickerFrag.glsl"); //using the colour picker shader
	checkerBoard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	checkerBoard->OnCreate();

	//Red Checker creation loop
	RowX = RowY = nextRow = 0;

	for (int x = 0; x <= 11; x++) {
		checkerRedList.push_back(std::make_shared<Actor>(checkerBoard.get()));
		checkerRedList[x]->AddComponent<TransformComponent>(nullptr, Vec3(-4.5f + RowX, -4.3f + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		checkerRedList[x]->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		checkerRedList[x]->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png"); //think about removing these
		checkerRedList[x]->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0;
			RowY += 1.26f;
			if (RowY == 1.26f) {
				RowX = 1.27f;
			}
		}
	}
	//Black Checker creation loop
	RowX = RowY = nextRow = 0;
	for (int x = 0; x <= 11; x++) {
		checkerBlackList.push_back(std::make_shared<Actor>(checkerBoard.get()));
		checkerBlackList[x]->AddComponent<TransformComponent>(nullptr, Vec3(-3.225f + RowX, 4.4f + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		checkerBlackList[x]->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		checkerBlackList[x]->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png"); //think about removing these
		checkerBlackList[x]->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0;
			RowY -= 1.26f;
			if (RowY == -1.26f) {
				RowX = -1.27f;
			}
		}
	}
	return true;
}

void Scene1::OnDestroy() {
	Debug::Info("Deleting assets Scene1: ", __FILE__, __LINE__);
	checkerRedList.clear();
	checkerBlackList.clear();
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {
	//ShaderComponent* shader = checkerRedList[0]->GetComponent<ShaderComponent>();
	switch (sdlEvent.type) {
	case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.0, 0.0, 0.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.0, 0.0, 0.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, 1.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, -1.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E) {
			checkerBoard->GetComponent<TransformComponent>()->SetTransform(checkerBoard->GetComponent<TransformComponent>()->GetPosition(), checkerBoard->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
			checkerBoard->GetComponent<TransformComponent>()->SetTransform(checkerBoard->GetComponent<TransformComponent>()->GetPosition(), checkerBoard->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		//The checker can be moved with WASD
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			if (pickedID < 24) {
				if (pickedID < 12) {
					checkerBlackList[pickedID]->GetComponent<TransformComponent>()->SetPosition(checkerBlackList[pickedID]->GetComponent<TransformComponent>()->GetPosition() + Vec3(0, 1.26, 0));
				}
				else {
					checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->SetPosition(checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->GetPosition() + Vec3(0, 1.26, 0)); //has the -11 because both lists have the length of 12 and picked id goes to 23
				}
			}
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			if (pickedID < 24) {
				if (pickedID < 12) {
					checkerBlackList[pickedID]->GetComponent<TransformComponent>()->SetPosition(checkerBlackList[pickedID]->GetComponent<TransformComponent>()->GetPosition() + Vec3(0, -1.26, 0));
				}
				else {
					checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->SetPosition(checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->GetPosition() + Vec3(0, -1.26, 0));
				}
			}
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			if (pickedID < 24) {
				if (pickedID < 12) {
					checkerBlackList[pickedID]->GetComponent<TransformComponent>()->SetPosition(checkerBlackList[pickedID]->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.26, 0, 0));
				}
				else {
					checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->SetPosition(checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.26, 0, 0));
				}
			}
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			if (pickedID < 24) {
				if (pickedID < 12) {
					checkerBlackList[pickedID]->GetComponent<TransformComponent>()->SetPosition(checkerBlackList[pickedID]->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.26, 0, 0));
				}
				else {
					checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->SetPosition(checkerRedList[pickedID - 11]->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.26, 0, 0));
				}
			}
		}
		break;

	case SDL_MOUSEMOTION:
		//checkerRedList[0]->GetComponent<TransformComponent>()->SetPosition(checkerRedList[0]->GetComponent<MouseMoveableComponent>()->getMouseVector(sdlEvent.button.x, sdlEvent.button.y, checkerRedList[0], camera->GetProjectionMatrix()));
		//checkerRedList[0]->GetComponent<MouseMoveableComponent>()->getMouseVector(sdlEvent.button.x, sdlEvent.button.y, checkerRedList[0]);
		break;

	case SDL_MOUSEBUTTONDOWN:
		int viewport[4];
		unsigned char data[4];
		glGetIntegerv(GL_VIEWPORT, viewport); //getting the viewport
		glReadPixels(sdlEvent.button.x, viewport[3] - sdlEvent.button.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); //man this was hard to figure out
		pickedID = data[0] + data[1] * 256 + data[2] * 256 * 256; //set the id for the checkers
		std::cout << pickedID << std::endl; //printing the ID
		//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glReadPixels.xhtml
		//the origin of SDL is bottom right, but for glReadPixels it's bottom left;
		break; 

	case SDL_MOUSEBUTTONUP:            
	break;

	default:
		break;
    }
}

void Scene1::Update(const float deltaTime) {
	
}

void Scene1::Render() const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderComponent* shader = checkerBoard->GetComponentRawPointer<ShaderComponent>();
	MeshComponent* mesh = checkerBoard->GetComponentRawPointer<MeshComponent>();
	MaterialComponent* texture = checkerBoard->GetComponentRawPointer<MaterialComponent>();
	if (shader == nullptr || mesh == nullptr || texture == nullptr) {
		return;
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBoard->GetModelMatrix());
	glUniform4f(shader->GetUniformID("PickingColor"), 24 / 255.0f, 24 / 255.0f, 24 / 255.0f, 1.0f); //setting the color of the board to white
	glBindBuffer(GL_UNIFORM_BUFFER, camera->GetMatriciesID());
	//glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID()); //for the texture shader
	//glBindTexture(GL_TEXTURE_2D, texture->getTextureID()); //for the texture shader
	mesh->Render(GL_TRIANGLES);

	for (int x = 0; x <= checkerRedList.size() - 1; x++) {
		int r = (x + 11 & 0x000000FF) >> 0; //using the red channel for each checker
		int g = (x + 11 & 0x0000FF00) >> 8; //every checker has a unique colour
		int b = (x + 11 & 0x00FF0000) >> 16;
		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(shader->GetUniformID("PickingColor"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f); //setting the color of the checker in the shader
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerRedList[x]->GetModelMatrix());
		//glBindTexture(GL_TEXTURE_2D, checkerRedList[x]->GetComponent<MaterialComponent>()->getTextureID()); //for the texture shader
		checkerRedList[x]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	for (int x = 0; x <= checkerBlackList.size() - 1; x++) {
		int r = (x & 0x000000FF) >> 0; //using the red channel for each checker
		int g = (x & 0x0000FF00) >> 8; //every checker has a unique colour
		int b = (x & 0x00FF0000) >> 16;
		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(shader->GetUniformID("PickingColor"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f); //setting the color of the checker in the shader
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBlackList[x]->GetModelMatrix());
		//glBindTexture(GL_TEXTURE_2D, checkerBlackList[x]->GetComponent<MaterialComponent>()->getTextureID()); //for the texture shader
		checkerBlackList[x]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	//glBindTexture(GL_TEXTURE_2D, 0); //for the texture shader
	glUseProgram(0);
}