#pragma once
#include "Condition.h"

class ConditionInRange : public Condition {
private:
	float thresholdDistance;
	Ref<Actor> target;
public:
	ConditionInRange(Ref<Actor> owner_, Ref<Actor> target_, float thresholdDistance_) : Condition{ owner_ } {
		target = target_;
		thresholdDistance = thresholdDistance_;
	}
	~ConditionInRange() {};
	bool Test();
};

