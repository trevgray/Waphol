#include "InRangeDecision.h"

#include "VMath.h"

bool InRangeDecision::TestValue() {
	//distance to player is less than some value
	if (VMath::distance(target->GetComponent<TransformComponent>()->GetPosition(), owner->GetComponent<TransformComponent>()->GetPosition()) < thresholdDistance) {
		return true;
	}
	return false;
}

InRangeDecision::~InRangeDecision() {
	
}
