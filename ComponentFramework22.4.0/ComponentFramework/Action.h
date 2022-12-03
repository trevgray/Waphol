#pragma once
#include "DecisionTreeNode.h"

//enum class ACTION_SET { SEEK = 0, DO_NOTHING };
class Action : public DecisionTreeNode {
private:
	std::string actionSet;
	//Ref<DecisionTreeNode> thisShared;
public:
	//Action(ACTION_SET actionSet_);
	Action(std::string actionSet_);
	virtual Ref<DecisionTreeNode> MakeDecision() override;
	virtual std::string GetValue() { return actionSet; }
	~Action();
};