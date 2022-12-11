#pragma once
#include "Decision.h"

class InRangeDecision : public Decision {
public:
	InRangeDecision(Ref<Actor> owner_, Ref<Actor> target_, float thresholdDistance_, DecisionTreeNode* trueNode_, DecisionTreeNode* falseNode_) {
		owner = owner_;
		target = target_;
		trueNode = trueNode_;
		falseNode = falseNode_;

		thresholdDistance = thresholdDistance_;
	}
	InRangeDecision(std::string ownerName_, std::string targetName_, float thresholdDistance_, DecisionTreeNode* trueNode_, DecisionTreeNode* falseNode_) {
		target = nullptr;
		ownerName = ownerName_;
		targetName = targetName_;
		trueNode = trueNode_;
		falseNode = falseNode_;

		thresholdDistance = thresholdDistance_;
	}
	bool TestValue() override {
		//distance to player is less than some value
		if (VMath::distance(target->GetComponent<TransformComponent>()->GetPosition(), owner->GetComponent<TransformComponent>()->GetPosition()) < thresholdDistance) {
			return true;
		}
		return false;
	}
	~InRangeDecision() {}
private:
	Ref<Actor> target;

	std::string ownerName;
	std::string targetName;
	float thresholdDistance;
};

