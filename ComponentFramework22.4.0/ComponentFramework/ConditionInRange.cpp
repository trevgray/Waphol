#include "ConditionInRange.h"
#include "VMath.h"

bool ConditionInRange::Test() {
    if (MATH::VMath::distance(target->GetComponent<TransformComponent>()->GetPosition(), owner->GetComponent<TransformComponent>()->GetPosition()) < thresholdDistance) {
        return true;
    }
    return false;
}
