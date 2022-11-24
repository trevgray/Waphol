#pragma once
#include "Component.h"
#include <string>
#include <vector>

#include "Decision.h" //which includes DecisionTreeNode.h
#include "InRangeDecision.h"
#include "Action.h"

class DecisionMakingComponent : public Component {
	DecisionMakingComponent(const DecisionMakingComponent&) = delete;
	DecisionMakingComponent(DecisionMakingComponent&&) = delete;
	DecisionMakingComponent& operator = (const DecisionMakingComponent&) = delete;
	DecisionMakingComponent& operator = (DecisionMakingComponent&&) = delete;
public:
	DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXML);
	virtual ~DecisionMakingComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	//std::string GetControllerTemplate() { return controllerTemplate; }
private:
	DecisionTreeNode* decider;
};