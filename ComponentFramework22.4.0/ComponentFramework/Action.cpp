#include "Action.h"
#include <iostream>

Action::Action(ACTION_SET actionSet_) {
	actionSet = actionSet_;
}

Ref<DecisionTreeNode> Action::MakeDecision() {
	if (actionSet == ACTION_SET::SEEK) {
		std::cout << "ACTION TEST" << std::endl;
	}
	return nullptr;
}

Action::~Action() {

}
