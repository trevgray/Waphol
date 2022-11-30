#include "DecisionMakingComponent.h"
#include "EngineManager.h"

DecisionMakingComponent::DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXML) : Component(parent_) {
	//for each decisionMakingXML, we make a StateMachine or DecisionTree
	decider = nullptr;
}

DecisionMakingComponent::~DecisionMakingComponent() {
	//delete decider;
}

bool DecisionMakingComponent::OnCreate() {
	//lets pretend the XML parsing produced these instances
	Ref<Action> trueNode = std::make_shared<Action>(ACTION_SET::SEEK);
	Ref<Action> falseNode = std::make_shared<Action>(ACTION_SET::DO_NOTHING);
	decider = std::make_shared<InRangeDecision>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), trueNode, falseNode);
	//-------------------------------------------------
	stateMachine = std::make_shared<StateMachine>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")); //this = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")

	Ref<State> seekPlayer = std::make_shared<State>(STATE::SEEK, ACTION_SET::SEEK);
	Ref<State>  doNothing = std::make_shared<State>(STATE::DO_NOTHING, ACTION_SET::DO_NOTHING);

	Ref<Condition> ifInRange = std::make_shared<ConditionInRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);
	doNothing->AddTransition(Transition(ifInRange, seekPlayer));
	Ref<Condition> ifOutOfRange = std::make_shared<ConditionOutOfRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);
	seekPlayer->AddTransition(Transition(ifOutOfRange, doNothing));
	stateMachine->SetInitialState(doNothing);

	return true;
}

void DecisionMakingComponent::OnDestroy() {}

void DecisionMakingComponent::Update(const float deltaTime_) {
	//call update for each StateMachine and DecisionTree
	//Ref<Action> a = std::dynamic_pointer_cast<Action>(decider->MakeDecision());
	stateMachine->Update();

}

void DecisionMakingComponent::Render() const {}
