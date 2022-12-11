#pragma once
#include "Condition.h"
#include "EngineManager.h"

class ConditionInRange : public Condition {
private:
	float thresholdDistance;
	Ref<Actor> target;
public:
	ConditionInRange(Ref<Actor> owner_, Ref<Actor> target_, float thresholdDistance_) : Condition{ owner_ } {
		target = target_;
		thresholdDistance = thresholdDistance_;
	}
	ConditionInRange(std::string owner_, std::string target_, float thresholdDistance_) {
		thresholdDistance = thresholdDistance_;
		SetOwner(EngineManager::Instance()->GetActorManager()->GetActor<Actor>(owner_));
		target = EngineManager::Instance()->GetActorManager()->GetActor<Actor>(target_);
	}
	~ConditionInRange() {}

	bool Test() {
		if (MATH::VMath::distance(target->GetComponent<TransformComponent>()->GetPosition(), owner->GetComponent<TransformComponent>()->GetPosition()) < thresholdDistance) {
			return true;
		}
		return false;
	}
};

