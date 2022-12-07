#pragma once
#include "Actor.h"
#include "State.h"
#include "Transition.h"
#include "Condition.h"

#include "ConditionInRange.h"
#include "ConditionOutOfRange.h"

class StateMachine {
private:
	Ref<State> initialState;
	Ref<State> currentState;
	Ref<Actor> owner;
public:
	StateMachine(Ref<Actor> owner_) { owner = owner_; }
	StateMachine(std::string owner_) {
		owner = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(owner_);
	}
	~StateMachine();
	void SetInitialState(Ref<State> initial_);
	void Update();

	STATE GetCurrentStateName() { return currentState->GetName(); }
};