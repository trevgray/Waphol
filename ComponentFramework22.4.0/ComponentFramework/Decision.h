#pragma once
#include "DecisionTreeNode.h"

class Decision : public DecisionTreeNode {
protected:
	Ref<DecisionTreeNode> trueNode;
	Ref<DecisionTreeNode> falseNode;
public:
	virtual Ref<DecisionTreeNode> MakeDecision() override;
	virtual bool TestValue();
	virtual Ref<DecisionTreeNode> GetBranch();
	~Decision();
};

