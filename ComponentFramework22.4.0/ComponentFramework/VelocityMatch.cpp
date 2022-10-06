#include "VelocityMatch.h"
#include "PhysicsBodyComponent.h"
#include "EngineManager.h"

VelocityMatch::VelocityMatch(Ref<Actor> target_, float timeToTarget_) {
	target = target_;
	timeToTarget = timeToTarget_;
}

VelocityMatch::VelocityMatch(std::string targetName_, float timeToTarget_) {
	targetName = targetName_;
	timeToTarget = timeToTarget_;
	target = nullptr;
}

VelocityMatch::~VelocityMatch() {

}

bool VelocityMatch::OnCreate() {
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second != nullptr && actor.first == targetName) {
			target = actor.second;
			break;
		}
	}
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

	result.rotation = Quaternion(-1, Vec3(0.0f, 0.0f, 0.0f));

	return result;
}