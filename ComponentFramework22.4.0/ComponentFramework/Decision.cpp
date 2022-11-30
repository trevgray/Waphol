#include "Decision.h"

Ref<DecisionTreeNode> Decision::MakeDecision() {
	Ref<DecisionTreeNode> branch = GetBranch();
	return branch->MakeDecision();
}

bool Decision::TestValue() {
	return false;
}

Ref<DecisionTreeNode> Decision::GetBranch() {
	if (TestValue()) {
		return trueNode;
	}
	return falseNode;
}

Decision::~Decision() {
	
}
