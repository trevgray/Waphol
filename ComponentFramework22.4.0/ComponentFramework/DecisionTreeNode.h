#pragma once
#include "Actor.h"

class DecisionTreeNode {
protected:
	Ref<Actor> owner;
public:
	DecisionTreeNode() {};
	virtual Ref<DecisionTreeNode> MakeDecision() { return nullptr; }
	~DecisionTreeNode() {}
};