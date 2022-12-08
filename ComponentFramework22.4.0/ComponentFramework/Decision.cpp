#include "Decision.h"

DecisionTreeNode* Decision::MakeDecision() {
	DecisionTreeNode* branch = GetBranch();
	return branch->MakeDecision();
}

bool Decision::TestValue() {
	return false;
}

DecisionTreeNode* Decision::GetBranch() {
	if (TestValue()) {
		return trueNode;
	}
	return falseNode;
}

Decision::~Decision() {
	if (trueNode) { delete trueNode; }
	if (falseNode) { delete falseNode; }
}
