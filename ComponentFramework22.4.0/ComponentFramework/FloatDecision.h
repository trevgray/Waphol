#pragma once
#include "Decision.h"

class FloatDecision : public Decision {
public:
	FloatDecision(float& reference_, float minValue_, float maxValue_, DecisionTreeNode* trueNode_, DecisionTreeNode* falseNode_) {
		reference = &reference_;
		minValue = minValue_;
		maxValue = maxValue_;
		trueNode = trueNode_;
		falseNode = falseNode_;
	}
	~FloatDecision() {}
	bool TestValue() override {
		if (maxValue >= *reference && *reference >= minValue) {
			return true;
		}
		return false;
	}
private:
	float* reference;
	float minValue;
	float maxValue;
};

