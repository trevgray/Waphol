#include "Action.h"
#include <iostream>

DecisionTreeNode* Action::MakeDecision() {
	if (actionSet == ACTION_SET::SEEK) {
		std::cout << "ACTION TEST" << std::endl;
	}
	return this;
}