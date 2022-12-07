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
	tinyxml2::XMLElement* currentBranch;
	std::string compareAttribute;
	//TRUE
	currentBranch = nodeElement->FirstChildElement("True");
	compareAttribute = currentBranch->Attribute("type");
	if (compareAttribute != "Action") {
		std::cout << currentBranch->Attribute("type") << std::endl;
		trueNode = MakeDecisionTreeNode(currentBranch); //pass it down the chain
	}
	else { //if the thing is a action, we create the node right here
		trueNode = std::make_shared<Action>(currentBranch->Attribute("actionSet"));
	}
	//FALSE
	currentBranch = nodeElement->FirstChildElement("False");
	compareAttribute = currentBranch->Attribute("type");
	if (compareAttribute != "Action") {
		falseNode = MakeDecisionTreeNode(currentBranch); //pass it down the chain
	}
	else { //if the thing is a action, we create the node right here
		falseNode = std::make_shared<Action>(currentBranch->Attribute("actionSet"));
	}

	//--CREATE THIS NODE--
	compareAttribute = nodeElement->Attribute("type");
	if (compareAttribute == "InRangeDecision") {
		//grab the names in the node and find refs to them
		thisNode = std::make_shared<InRangeDecision>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeElement->Attribute("ownerName")),
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeElement->Attribute("targetName")), trueNode, falseNode);
	}
	else {
		std::cout << "DECISION TREE TYPE NOT FOUND" << std::endl;
		thisNode = nullptr;
	}
	return thisNode;
}

StateMachine DecisionMakingComponent::MakeStateMachine(tinyxml2::XMLElement* stateMachineElement) {
	StateMachine thisStateMachine = StateMachine(stateMachineElement->Attribute("owner"));
	//State Machine Utility
	std::unordered_map<std::string, Ref<Condition>> conditions;
	std::unordered_map<std::string, Ref<State>> states;
	tinyxml2::XMLElement* currentElement;
	//States Loop - Get all the states in a hash table
	currentElement = stateMachineElement->FirstChildElement("State"); //loading first State from stateMachine Scope
	bool stateLoop = true;
	while (stateLoop) {
		std::string stateCheck = currentElement->Name();
		if (stateCheck == "State") { //check if the element is a state
			states[currentElement->Attribute("name")] = std::make_shared<State>(static_cast<STATE>(currentElement->Int64Attribute("state")), currentElement->Attribute("actionSet"));
			//Exit if we are at the last element
			if (currentElement == stateMachineElement->LastChildElement("State")) { //stopping looping when the current element is the last element in Scene Scope - stateMachineElement->LastChild() will also work, but stopping at the last element should be faster
				stateLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement(); //loading the next element
	}
	//Conditions Loop - Get all the conditions in a hash table - same as State, but create unique conditions based on type
	currentElement = stateMachineElement->FirstChildElement("Condition");
	bool conditionLoop = true;
	while (conditionLoop) {
		std::string conditionCheck = currentElement->Name();
		if (conditionCheck == "Condition") {
			std::string conditionType = currentElement->Attribute("type");
			//Create unique conditions based on type
			if (conditionType == "ConditionInRange") { //ConditionInRange
				conditions[currentElement->Attribute("name")] = std::make_shared<ConditionInRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
					EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);
			}
			else if (conditionType == "ConditionOutOfRange") { //ConditionOutOfRange
				conditions[currentElement->Attribute("name")] = std::make_shared<ConditionInRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
					EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);
			}
			//Exit if we are at the last element
			if (currentElement == stateMachineElement->LastChildElement("Condition")) {
				conditionLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement();
	}
	//Transitions loop - make all the transitions on the states
	currentElement = stateMachineElement->FirstChildElement("Transition");
	bool transitionLoop = true;
	while (transitionLoop) {
		std::string transitionCheck = currentElement->Name();
		if (transitionCheck == "Transition") { //check if the element is a Transition
			//add the Transition to the state (this is why we set up all the hash tables)
			states[currentElement->Attribute("state")]->AddTransition(Transition(conditions[currentElement->Attribute("condition")], states[currentElement->Attribute("targetState")]));
			//Exit if we are at the last element
			if (currentElement == stateMachineElement->LastChildElement("Transition")) {
				transitionLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement(); //loading the next component
	}
	//Set initial state
	thisStateMachine.SetInitialState(states[stateMachineElement->FirstChildElement("InitialState")->Attribute("name")]);
	return thisStateMachine;
}

bool DecisionMakingComponent::OnCreate() {
	tinyxml2::XMLDocument XMLFile;
	XMLFile.LoadFile(decisionMakingXMLs[0].c_str()); //loading XML file
	if (XMLFile.Error()) { //Error detection in the xml
		std::cout << XMLFile.ErrorIDToName(XMLFile.ErrorID()) << std::endl;
		return false;
	}
	tinyxml2::XMLElement* currentElement = nullptr;

	currentElement = XMLFile.RootElement()->FirstChildElement("DecisionTree");

	decisionTrees.push_back(MakeDecisionTreeNode(currentElement->FirstChildElement("RootNode")));

	//for each decisionMakingXML, we make a each StateMachine or DecisionTree


	//lets pretend the XML parsing produced these instances
	/*Ref<Action> trueNode = std::make_shared<Action>(ACTION_SET::SEEK);
	Ref<Action> falseNode = std::make_shared<Action>(ACTION_SET::DO_NOTHING);
	decisionTrees.push_back(std::make_shared<InRangeDecision>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), trueNode, falseNode));*/

	//-------------------------------------------------

	currentElement = XMLFile.RootElement()->FirstChildElement("StateMachine");

	stateMachines.push_back(MakeStateMachine(currentElement));

	/*stateMachines.push_back(StateMachine("NPC")); //this = EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC")

	Ref<State> seekPlayer = std::make_shared<State>(STATE::SEEK, "SEEK");
	Ref<State> doNothing = std::make_shared<State>(STATE::DO_NOTHING, "DO_NOTHING");

	Ref<Condition> ifInRange = std::make_shared<ConditionInRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);

	doNothing->AddTransition(Transition(ifInRange, seekPlayer));

	Ref<Condition> ifOutOfRange = std::make_shared<ConditionOutOfRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>("NPC"),
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>("Player"), 5.0f);

	seekPlayer->AddTransition(Transition(ifOutOfRange, doNothing));

	stateMachines[0].SetInitialState(doNothing);*/
	return true;
}

void DecisionMakingComponent::OnDestroy() {}

void DecisionMakingComponent::Update(const float deltaTime_) {
	//Ref<Action> a = std::dynamic_pointer_cast<Action>(decider->MakeDecision());
	//call update for each StateMachine and DecisionTree
	for (Ref<DecisionTreeNode> decisionTree : decisionTrees) {
		//decisionTree->MakeDecision();
	}
	for (StateMachine stateMachine : stateMachines) {
		stateMachine.Update();
	}
}

void DecisionMakingComponent::Render() const {}
