#pragma once
#include "Component.h"
#include <string>
#include <vector>

#include "Decision.h" //which includes DecisionTreeNode.h
#include "InRangeDecision.h"
#include "Action.h"

#include "StateMachine.h"

#include "ActionManager.h"

#include "tinyxml2.h"

class DecisionMakingComponent : public Component {
	DecisionMakingComponent(const DecisionMakingComponent&) = delete;
	DecisionMakingComponent(DecisionMakingComponent&&) = delete;
	DecisionMakingComponent& operator = (const DecisionMakingComponent&) = delete;
	DecisionMakingComponent& operator = (DecisionMakingComponent&&) = delete;
public:
	DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXMLs_);
	virtual ~DecisionMakingComponent();
	DecisionTreeNode* MakeDecisionTreeNode(tinyxml2::XMLElement* nodeElement);
	StateMachine MakeStateMachine(tinyxml2::XMLElement* stateMachineElement);
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	//std::string GetControllerTemplate() { return controllerTemplate; }
private:
	std::vector<std::string> decisionMakingXMLs;
	//Decision Trees
	std::vector<DecisionTreeNode*> decisionTrees;
	//State Machines
	std::vector <StateMachine> stateMachines;

	ActionManager actionManager;
};