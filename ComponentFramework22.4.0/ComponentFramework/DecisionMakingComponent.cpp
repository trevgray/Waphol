#include "DecisionMakingComponent.h"
#include "EngineManager.h"

DecisionMakingComponent::DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXML) : Component(parent_) {
	//for each decisionMakingXML, we make a StateMachine or DecisionTree
}

DecisionMakingComponent::~DecisionMakingComponent() {

}

bool DecisionMakingComponent::OnCreate() {
	//lets pretend the XML parsing produced these instances
	DecisionTreeNode* trueNode = new Action(ACTION_SET::SEEK);
	DecisionTreeNode* falseNode = new Action(ACTION_SET::DO_NOTHING);
	decider = new InRangeDecision(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"), 
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), trueNode, falseNode);

	return true;
}

void DecisionMakingComponent::OnDestroy() {}

void DecisionMakingComponent::Update(const float deltaTime_) {
	//call update for each StateMachine and DecisionTree
	//if (decider != nullptr) {
		DecisionTreeNode* action = decider->MakeDecision();
		Action* a = dynamic_cast<Action*>(action);
		switch (a->GetValue()) {
		case ACTION_SET::SEEK:
			//SteerToSeekPlayer(steering);
			break;
		case ACTION_SET::DO_NOTHING:
			break;
		}
	//}
}

void DecisionMakingComponent::Render() const {}
