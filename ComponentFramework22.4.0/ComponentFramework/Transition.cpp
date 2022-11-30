#include "Transition.h"
Transition::Transition() {
	condition = nullptr;
	targetState = nullptr;
}


Transition::Transition(Ref<Condition> condition_, Ref<State> targetState_) {
	condition = condition_;
	targetState = targetState_;
}

Transition::~Transition() {}

bool Transition::IsTriggered() {
	return condition->Test();
}
