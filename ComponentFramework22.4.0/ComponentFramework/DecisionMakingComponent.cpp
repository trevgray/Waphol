#include "DecisionMakingComponent.h"

DecisionMakingComponent::DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXML) : Component(parent_) {
	//for each decisionMakingXML, we make a StateMachine or DecisionTree
}

DecisionMakingComponent::~DecisionMakingComponent() {

}

bool DecisionMakingComponent::OnCreate() {
	return true;
}

void DecisionMakingComponent::OnDestroy() {}

void DecisionMakingComponent::Update(const float deltaTime_) {
	//call update for each StateMachine and DecisionTree
}

void DecisionMakingComponent::Render() const {}
