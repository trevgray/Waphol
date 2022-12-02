#pragma once
#include "Component.h"
#include <string>
#include <vector>

#include "Decision.h" //which includes DecisionTreeNode.h
#include "InRangeDecision.h"
#include "Action.h"

#include "StateMachine.h"

#include "tinyxml2.h"

class DecisionMakingComponent : public Component {
	DecisionMakingComponent(const DecisionMakingComponent&) = delete;
	DecisionMakingComponent(DecisionMakingComponent&&) = delete;
	DecisionMakingComponent& operator = (const DecisionMakingComponent&) = delete;
	DecisionMakingComponent& operator = (DecisionMakingComponent&&) = delete;
public:
	DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXMLs_);
	virtual ~DecisionMakingComponent();
	Ref<DecisionTreeNode> MakeDecisionTreeNode(tinyxml2::XMLElement* nodeElement);
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	//std::string GetControllerTemplate() { return controllerTemplate; }
private:
	std::vector<std::string> decisionMakingXMLs;

	std::vector<Ref<DecisionTreeNode>> decisionTrees;
	std::vector <StateMachine> stateMachines;
};