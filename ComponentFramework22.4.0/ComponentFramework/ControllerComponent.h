#pragma once
#include "Component.h"
#include <iostream>

class ControllerComponent : public Component {
	ControllerComponent(const ControllerComponent&) = delete;
	ControllerComponent(ControllerComponent&&) = delete;
	ControllerComponent& operator = (const ControllerComponent&) = delete; 
	ControllerComponent& operator = (ControllerComponent&&) = delete;
public:
	ControllerComponent(Component* parent_, std::string controllerTemplate_);
	virtual ~ControllerComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	std::string GetControllerTemplate() { return controllerTemplate; }
private:
	std::string controllerTemplate;
};