#pragma once
#include "Condition.h"
class AndCondition : public Condition {
public:
	AndCondition(Ref<Condition> conditionA_, Ref<Condition> conditionB_) : Condition(nullptr) {
		conditionA = conditionA_;
		conditionB = conditionB_;
	}
	~AndCondition() {}
	bool Test() {
		if (conditionA->Test() == true && conditionB->Test() == true) {
			return true;
		}
		return false;
	}

private:
	Ref<Condition> conditionA;
	Ref<Condition> conditionB;
};

