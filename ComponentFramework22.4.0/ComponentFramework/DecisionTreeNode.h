#pragma once
#include "Actor.h"

class DecisionTreeNode {
protected:
	Ref<Actor> owner;
public:
	DecisionTreeNode() {};
	virtual DecisionTreeNode* MakeDecision() { return nullptr; }
	virtual ~DecisionTreeNode() {}
};