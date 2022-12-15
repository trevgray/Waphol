#pragma once
#include "DecisionTreeNode.h"

class Decision : public DecisionTreeNode {
protected:
	DecisionTreeNode* trueNode;
	DecisionTreeNode* falseNode;
public:
	virtual DecisionTreeNode* MakeDecision() override;
	virtual bool TestValue();
	virtual DecisionTreeNode* GetBranch();
	virtual ~Decision();
};

