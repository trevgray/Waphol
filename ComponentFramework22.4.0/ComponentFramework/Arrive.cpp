#include "Arrive.h"
#include "PhysicsBodyComponent.h"

Arrive::Arrive(Ref<TransformComponent> target_) {
	target = target_;
	targetRadius = 2.0f;
	slowRadius = 10.0f;
	timeToTarget = 0.1f;
}

Arrive::Arrive(Ref<TransformComponent> target_, float targetRadius_, float slowRadius_, float timeToTarget_) {
	target = target_;
	targetRadius = targetRadius_;
	slowRadius = slowRadius_;
	timeToTarget = timeToTarget_;
}

Arrive::~Arrive() {

}

SteeringOutput Arrive::GetSteering(Ref<Actor> actor_) {
	//get direction to target
	Vec3 direction = target->GetPosition() - actor_->GetComponent<TransformComponent>()->GetPosition();
	float distance = VMath::mag(direction);
	// check if we are there, return no steering
	if (distance < targetRadius) {
		actor_->GetComponent<PhysicsBodyComponent>()->SetVel(Vec3());
		result.linear = Vec3();
		result.rotation = Quaternion(0, Vec3());
		std::cout << "STOPPED" << std::endl;
		return result;
	}
	// if we are outside the slow radius, use max speed
	float targetSpeed;
	if (distance > slowRadius) {
		targetSpeed = actor_->GetComponent<PhysicsBodyComponent>()->GetMaxSpeed();
	}
	// use scaled speed
	else {
		targetSpeed = actor_->GetComponent<PhysicsBodyComponent>()->GetMaxSpeed() * distance / slowRadius;
	}
	// The target velocity combines speed and direction
	Vec3 targetVelocity = direction;
	targetVelocity = VMath::normalize(targetVelocity);
	targetVelocity *= targetSpeed;
	// Acceleration tries to get to the target velocity
	result.linear = targetVelocity - actor_->GetComponent<PhysicsBodyComponent>()->GetVel();
	result.linear /= timeToTarget;
	// Clip acceleration if too high
	if (VMath::mag(result.linear) > actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration()) {
		result.linear = VMath::normalize(result.linear);
		result.linear *= actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration();
	}
	result.rotation = Quaternion(0, Vec3());
	std::cout << "MOVING" << std::endl;
	return result;
}