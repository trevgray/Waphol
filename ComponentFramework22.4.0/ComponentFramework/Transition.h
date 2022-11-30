#pragma once
#include "State.h"
#include "Condition.h"

class Transition {
private:
	//ignoring action for this demo
	Ref<State> targetState;
	Ref<Condition> condition;
public:
	Transition();
	Transition(Ref<Condition> condition_, Ref<State> targetState_);
	~Transition();
	bool IsTriggered();
	Ref<State> GetTargetState() { return targetState; }
};