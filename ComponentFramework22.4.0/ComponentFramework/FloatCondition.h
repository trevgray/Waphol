#pragma once
#include "Condition.h"
class FloatCondition : public Condition {
public:
	FloatCondition(float& reference_, float minValue_, float maxValue_) : Condition(nullptr) {
		minValue = minValue_;
		maxValue = maxValue_;
		reference = &reference_;
	}
	~FloatCondition() {}
	bool Test() {
		if (maxValue >= *reference && *reference >= minValue) {
			return true;
		}
		return false;
	}
private:
	float minValue;
	float maxValue;
	float* reference;
};

