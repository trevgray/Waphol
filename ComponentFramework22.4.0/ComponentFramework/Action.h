#pragma once
#include "DecisionTreeNode.h"

enum class ACTION_SET { SEEK = 0, DO_NOTHING };

class Action : public DecisionTreeNode {
private:
	ACTION_SET actionSet;
	//Ref<DecisionTreeNode> thisShared;
public:
	Action(ACTION_SET actionSet_);
	virtual Ref<DecisionTreeNode> MakeDecision() override;
	virtual ACTION_SET GetValue() { return actionSet; }
	~Action();
};