#pragma once
#include "DecisionTreeNode.h"

//enum class ACTION_SET { SEEK = 0, DO_NOTHING };
class Action : public DecisionTreeNode {
private:
	std::string actionSet;
	//Ref<DecisionTreeNode> thisShared;

	float expiryTime;
	float executionTime;
public:
	unsigned int priority;
	//Action(ACTION_SET actionSet_);
	Action(std::string actionSet_, unsigned int priority_, float expiryTime_, float executionTime_);
	~Action();

	bool isExecuted;

	virtual DecisionTreeNode* MakeDecision() override;
	virtual std::string GetValue() { return actionSet; }
	//virtual void Execute();

	bool IsComplete(float totalTime);
	bool IsExpired(float totalTime);
};