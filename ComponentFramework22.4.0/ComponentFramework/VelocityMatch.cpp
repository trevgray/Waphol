#include "VelocityMatch.h"
#include "PhysicsBodyComponent.h"

VelocityMatch::VelocityMatch(Ref<Actor> target_, float timeToTarget_) {
	target = target_;
	timeToTarget = timeToTarget_;
}

VelocityMatch::~VelocityMatch() {

}

bool VelocityMatch::OnCreate() {
	return true;
}

SteeringOutput VelocityMatch::GetSteering(Ref<Actor> actor_) {
	// Acceleration tries to get to the target velocity
	result.linear = target->GetComponent<PhysicsBodyComponent>()->GetVel() - actor_->GetComponent<PhysicsBodyComponent>()->GetVel();
	result.linear /= timeToTarget;

	// Clip to max acceleration if needed
	if (VMath::mag(result.linear) > actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration()) {
		result.linear = VMath::normalize(result.linear);
		result.linear *= actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration();
	}

	result.rotation = Quaternion();

	return result;
}