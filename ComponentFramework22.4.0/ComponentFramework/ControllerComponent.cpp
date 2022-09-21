#include "ControllerComponent.h"

ControllerComponent::ControllerComponent(Component* parent_, std::string controllerTemplate_) : Component(parent_), controllerTemplate(controllerTemplate_) {}

ControllerComponent::~ControllerComponent() {
	OnDestroy();
}

bool ControllerComponent::OnCreate() {
	if (controllerTemplate.size() != 0) {
		isCreated = true;
	}
	return isCreated;
}

void ControllerComponent::OnDestroy() {
	controllerTemplate.clear();
	isCreated = false;
}

void ControllerComponent::Update(const float deltaTime) {}

void ControllerComponent::Render()const {}