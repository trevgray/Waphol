#include "Action.h"
#include <iostream>

//Action::Action(ACTION_SET actionSet_) {
//	actionSet = actionSet_;
//}

Action::Action(std::string actionSet_, float expiryTime_, float executionTime_) {
	actionSet = actionSet_;
	expiryTime = expiryTime_;
	executionTime = executionTime_;
	isExecuted = false;
}

Action::~Action() {}


DecisionTreeNode* Action::MakeDecision() {
	return this;
}

void Action::Execute() {
	isExecuted = true;
	if (actionSet == "SEEK") {
		std::cout << "ACTION TEST" << std::endl;
	}
}

bool Action::IsComplete(float totalTime) {
	if (totalTime > executionTime) {
		isExecuted = false;
		return true;
	}
	return false;
}

bool Action::IsExpired(float totalTime) {
	if (totalTime > expiryTime) {
		return true;
	}
	return false;
}
