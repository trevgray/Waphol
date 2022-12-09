#include "DecisionMakingComponent.h"
#include "EngineManager.h"

DecisionMakingComponent::DecisionMakingComponent(Component* parent_, std::vector<std::string> decisionMakingXMLs_) : Component(parent_) {
	decisionMakingXMLs = decisionMakingXMLs_;
}

DecisionMakingComponent::~DecisionMakingComponent() {
	//delete decider;
}

DecisionTreeNode* DecisionMakingComponent::MakeDecisionTreeNode(tinyxml2::XMLElement* nodeElement) { //
	//when we find a test, we have the info for both the branches, we finally make that decisiontreenode after that
	//grab the names in the Decision tree and find refs to them
	//then create the decisiontreenode
	//recursively call the function if there are any test elements in the node element in both true and false

	DecisionTreeNode* thisNode;
	DecisionTreeNode* trueNode;
	DecisionTreeNode* falseNode;

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
		trueNode = new Action(currentBranch->Attribute("actionSet"), currentBranch->Int64Attribute("priority"), currentBranch->FloatAttribute("expiryTime"), currentBranch->FloatAttribute("executionTime"));
	}
	//FALSE
	currentBranch = nodeElement->FirstChildElement("False");
	compareAttribute = currentBranch->Attribute("type");
	if (compareAttribute != "Action") {
		falseNode = MakeDecisionTreeNode(currentBranch); //pass it down the chain
	}
	else { //if the thing is a action, we create the node right here
		falseNode = new Action(currentBranch->Attribute("actionSet"), currentBranch->Int64Attribute("priority"), currentBranch->FloatAttribute("expiryTime"), currentBranch->FloatAttribute("executionTime"));
	}

	//--CREATE THIS NODE--
	compareAttribute = nodeElement->Attribute("type");
	if (compareAttribute == "InRangeDecision") {
		//grab the names in the node and find refs to them
		thisNode = new InRangeDecision(EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeElement->Attribute("ownerName")),
			EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeElement->Attribute("targetName")), nodeElement->FloatAttribute("thresholdDistance"), trueNode, falseNode);
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
			states[currentElement->Attribute("name")] = std::make_shared<State>(static_cast<STATE>(currentElement->Int64Attribute("state")), currentElement->Attribute("actionSet"), currentElement->Int64Attribute("priority"), currentElement->FloatAttribute("expiryTime"), currentElement->FloatAttribute("executionTime"));
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
				conditions[currentElement->Attribute("name")] = std::make_shared<ConditionInRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>(currentElement->Attribute("owner")),
					EngineManager::Instance()->GetActorManager()->GetActor<Actor>(currentElement->Attribute("target")), currentElement->FloatAttribute("thresholdDistance"));
			}
			else if (conditionType == "ConditionOutOfRange") { //ConditionOutOfRange
				conditions[currentElement->Attribute("name")] = std::make_shared<ConditionInRange>(EngineManager::Instance()->GetActorManager()->GetActor<Actor>(currentElement->Attribute("owner")),
					EngineManager::Instance()->GetActorManager()->GetActor<Actor>(currentElement->Attribute("target")), currentElement->FloatAttribute("thresholdDistance"));
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
	//find parent actor
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second->GetComponent<DecisionMakingComponent>() != nullptr && actor.second->GetComponent<DecisionMakingComponent>().get() == this) {
			actionManager.SetOwner(actor.second); //Set owner in the action Manager
			break;
		}
	}
	//Build the decision making structures
	tinyxml2::XMLDocument XMLFile;
	//for each decisionMakingXML, we make each StateMachine or DecisionTree
	for (std::string fileName : decisionMakingXMLs) {
		XMLFile.LoadFile(fileName.c_str()); //loading XML file
		if (XMLFile.Error()) { //Error detection in the xml
			std::cout << XMLFile.ErrorIDToName(XMLFile.ErrorID()) << std::endl;
			return false;
		}
		tinyxml2::XMLElement* currentElement = nullptr;
		//DecisionTree loop
		currentElement = XMLFile.RootElement()->FirstChildElement("DecisionTree");
		if (currentElement != nullptr) {
			bool decisionLoop = true;
			while (decisionLoop) {
				std::string decisionCheck = currentElement->Name();
				if (decisionCheck == "DecisionTree") { //check if the element is a Transition
					//add the Transition to the state (this is why we set up all the hash tables)
					decisionTrees.push_back(MakeDecisionTreeNode(currentElement->FirstChildElement("RootNode")));
					//Exit if we are at the last element
					if (currentElement == XMLFile.RootElement()->LastChildElement("DecisionTree")) {
						decisionLoop = false;
					}
				}
				currentElement = currentElement->NextSiblingElement(); //loading the next component
			}
		}
		//decisionTrees.push_back(MakeDecisionTreeNode(currentElement->FirstChildElement("RootNode")));

		//StateMachine loop
		currentElement = XMLFile.RootElement()->FirstChildElement("StateMachine");
		if (currentElement != nullptr) {
			bool stateLoop = true;
			while (stateLoop) {
				std::string stateCheck = currentElement->Name();
				if (stateCheck == "StateMachine") { //check if the element is a Transition
					//add the Transition to the state (this is why we set up all the hash tables)
					stateMachines.push_back(MakeStateMachine(currentElement));
					//Exit if we are at the last element
					if (currentElement == XMLFile.RootElement()->LastChildElement("StateMachine")) {
						stateLoop = false;
					}
				}
				currentElement = currentElement->NextSiblingElement(); //loading the next component
			}
		}
		//stateMachines.push_back(MakeStateMachine(currentElement));
	}
	return true;
}

void DecisionMakingComponent::OnDestroy() {}

void DecisionMakingComponent::Update(const float deltaTime_) {
	//Ref<Action> a = std::dynamic_pointer_cast<Action>(decider->MakeDecision());
	//call update for each StateMachine and DecisionTree
	for (DecisionTreeNode* decisionTree : decisionTrees) {
		actionManager.ScheduleAction(dynamic_cast<Action*>(decisionTree->MakeDecision()));
	}
	for (StateMachine stateMachine : stateMachines) {
		actionManager.ScheduleAction(stateMachine.Update());
	}
	actionManager.Execute(deltaTime_);
}

void DecisionMakingComponent::Render() const {}
