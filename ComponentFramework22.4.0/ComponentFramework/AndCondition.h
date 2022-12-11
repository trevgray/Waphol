#pragma once
#include "Condition.h"
class AndCondition : public Condition {
public:
	AndCondition(Condition conditionA_, Condition conditionB_) : Condition(nullptr) {
		conditionA = conditionA_;
		conditionB = conditionB_;
	}
	~AndCondition() {}
	bool Test() {
		if (conditionA.Test() == true && conditionB.Test() == true) {
			return true;
		}
		return false;
	}

private:
	Condition conditionA;
	Condition conditionB;
};

