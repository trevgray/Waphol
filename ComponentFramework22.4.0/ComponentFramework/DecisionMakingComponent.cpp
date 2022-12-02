#include "DecisionMakingComponent.h"
#include "EngineManager.h"

DecisionMakingComponent::DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXMLs_) : Component(parent_) {
	decisionMakingXMLs = decisionMakingXMLs_;
}

DecisionMakingComponent::~DecisionMakingComponent() {
	//delete decider;
}

Ref<DecisionTreeNode> DecisionMakingComponent::MakeDecisionTreeNode(tinyxml2::XMLElement* nodeElement) { //
	//when we find a test, we have the info for both the branches, we finally make that decisiontreenode after that
	//grab the names in the Decision tree and find refs to them
	//then create the decisiontreenode
	//recursively call the function if there are any test elements in the node element in both true and false

	Ref<DecisionTreeNode> thisNode;
	Ref<DecisionTreeNode> trueNode;
	Ref<DecisionTreeNode> falseNode;

	//--BRANCHES--
	//TRUE
	tinyxml2::XMLElement* currentBranch;
	std::string compareAttribute;

	currentBranch = nodeElement->FirstChildElement("True");
	compareAttribute = currentBranch->Attribute("type");
	if (compareAttribute != "Action") {
		std::cout << currentBranch->Attribute("type") << std::endl;
		trueNode = MakeDecisionTreeNode(currentBranch); //pass it down the chain
	}
	else { //if the thing is a action, we create the node right here
		trueNode = std::make_shared<Action>(ACTION_SET::SEEK);
	}
	//FALSE
	currentBranch = nodeElement->FirstChildElement("False");
	compareAttribute = currentBranch->Attribute("type");
	if (compareAttribute != "Action") {
		falseNode = MakeDecisionTreeNode(currentBranch); //pass it down the chain
	}
	else { //if the thing is a action, we create the node right here
		falseNode = std::make_shared<Action>(ACTION_SET::DO_NOTHING);
	}

	//--CREATE THIS NODE--
	compareAttribute = nodeElement->Attribute("type");
	if (compareAttribute == "InRangeDecision") {
		//grab the names in the node and find refs to them
		thisNode = std::make_shared<InRangeDecision>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), trueNode, falseNode);
	}
	else {
		std::cout << "DECISION TREE TYPE NOT FOUND" << std::endl;
		thisNode = nullptr;
	}
	return thisNode;
}

bool DecisionMakingComponent::OnCreate() {
	tinyxml2::XMLDocument XMLFile;
	tinyxml2::XMLElement* decisionRoot = nullptr;
	tinyxml2::XMLElement* currentElement = nullptr;

	XMLFile.LoadFile(decisionMakingXMLs[0].c_str()); //loading XML file
	if (XMLFile.Error()) { //Error detection in the xml
		std::cout << XMLFile.ErrorIDToName(XMLFile.ErrorID()) << std::endl;
		return false;
	}
	decisionRoot = XMLFile.RootElement()->FirstChildElement("DecisionTree");
	currentElement = decisionRoot->FirstChildElement("RootNode");

	decisionTrees.push_back(MakeDecisionTreeNode(currentElement));

	//for each decisionMakingXML, we make a each StateMachine or DecisionTree


	//lets pretend the XML parsing produced these instances
	/*Ref<Action> trueNode = std::make_shared<Action>(ACTION_SET::SEEK);
	Ref<Action> falseNode = std::make_shared<Action>(ACTION_SET::DO_NOTHING);
	decisionTrees.push_back(std::make_shared<InRangeDecision>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), trueNode, falseNode));*/

	//-------------------------------------------------

	stateMachines.push_back(StateMachine(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"))); //this = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")

	Ref<State> seekPlayer = std::make_shared<State>(STATE::SEEK, ACTION_SET::SEEK);
	Ref<State>  doNothing = std::make_shared<State>(STATE::DO_NOTHING, ACTION_SET::DO_NOTHING);

	Ref<Condition> ifInRange = std::make_shared<ConditionInRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);
	doNothing->AddTransition(Transition(ifInRange, seekPlayer));
	Ref<Condition> ifOutOfRange = std::make_shared<ConditionOutOfRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);
	seekPlayer->AddTransition(Transition(ifOutOfRange, doNothing));
	stateMachines[0].SetInitialState(doNothing);

	return true;
}

void DecisionMakingComponent::OnDestroy() {}

void DecisionMakingComponent::Update(const float deltaTime_) {
	//Ref<Action> a = std::dynamic_pointer_cast<Action>(decider->MakeDecision());
	//call update for each StateMachine and DecisionTree
	for (Ref<DecisionTreeNode> decisionTree : decisionTrees) {
		decisionTree->MakeDecision();
	}
	for (StateMachine stateMachine : stateMachines) {
		//stateMachine.Update();
	}
}

void DecisionMakingComponent::Render() const {}
