#include "ConditionOutOfRange.h"
#include "VMath.h"

bool ConditionOutOfRange::Test() {
    float thresholdDistance = 3.5f;
    if (MATH::VMath::distance(target->GetComponent<TransformComponent>()->GetPosition(), owner->GetComponent<TransformComponent>()->GetPosition()) > thresholdDistance) {
        return true;
    }
    return false;
}
