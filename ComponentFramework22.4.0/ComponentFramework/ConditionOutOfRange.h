#pragma once
#include "Condition.h"

class ConditionOutOfRange : public Condition {
private:
	float thresholdDistance;
	Ref<Actor> target;
public:
	ConditionOutOfRange(Ref<Actor> owner_, Ref<Actor> target_, float thresholdDistance_) : Condition{ owner_ } {
		target = target_;
		thresholdDistance = thresholdDistance_;
	}
	~ConditionOutOfRange() {}
	bool Test();
};

