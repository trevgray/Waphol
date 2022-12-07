#pragma once
#include "Condition.h"
#include "EngineManager.h"

class ConditionOutOfRange : public Condition {
private:
	float thresholdDistance;
	Ref<Actor> target;
public:
	ConditionOutOfRange(Ref<Actor> owner_, Ref<Actor> target_, float thresholdDistance_) : Condition{ owner_ } {
		target = target_;
		thresholdDistance = thresholdDistance_;
	}
	ConditionOutOfRange(std::string owner_, std::string target_, float thresholdDistance_) {
		thresholdDistance = thresholdDistance_;
		SetOwner(EngineManager::Instance()->GetActorManager()->GetActor<Actor>(owner_));
		target = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(target_);
	}
	~ConditionOutOfRange() {}
	bool Test();
};

