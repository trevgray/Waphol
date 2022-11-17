#include "MaterialComponent.h"
#include "EngineManager.h"

MaterialComponent::MaterialComponent(Component* parent_, std::string filename_):Component(parent_), filename(filename_), textureID(0) {}

MaterialComponent::~MaterialComponent() {
	OnDestroy();
}

bool MaterialComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	EngineManager::Instance()->GetRenderer()->LoadTexture2D(filename, textureID);
	return isCreated;
}

void MaterialComponent::OnDestroy() {
	EngineManager::Instance()->GetRenderer()->RemoveTexture2D(textureID);
	isCreated = false;
}

void MaterialComponent::Update(const float deltaTime) {}

void MaterialComponent::Render()const {}

bool MaterialComponent::SetNewTexture(std::string filename) {
	EngineManager::Instance()->GetRenderer()->LoadTexture2D(filename, textureID);
	return true;
}
