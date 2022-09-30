#include "Align.h"
#include "PhysicsBodyComponent.h"

Align::Align(Ref<TransformComponent> target_, Quaternion baseq_, float targetRadius_, float slowRadius_, float timeToTarget_) {
	target = target_;
	baseq = baseq_;
	targetRadius = targetRadius_;
	slowRadius = slowRadius_;
	timeToTarget = timeToTarget_;
}

Align::~Align() {

}

SteeringOutput Align::GetSteering(Ref<Actor> actor_) {
	Quaternion currentQ = actor_->GetComponent<TransformComponent>()->GetQuaternion() * -baseq;
	float angleBase = (acos(currentQ.w) * RADIANS_TO_DEGREES) * 2;
	
	Vec3 direction = target->GetPosition() - actor_->GetComponent<TransformComponent>()->GetPosition();
	float angleDirection = atan2(direction.y, direction.x) * RADIANS_TO_DEGREES;
	std::cout << angleDirection << std::endl;

	//float rotation = mapToRange(angleBase - angleDirection);

	//actor_->GetComponent<PhysicsBodyComponent>()->SetRotation(rotation);
	float rotation = mapToRange(angleBase - angleDirection);
	float rotationSize = abs(rotation);

	if (rotationSize < targetRadius) {
		return SteeringOutput();
	}

	float targetRotation;
	if (rotationSize > slowRadius) {
		targetRotation = actor_->GetComponent<PhysicsBodyComponent>()->GetMaxRotation();
	}
	else {
		// use scaled rotation speed
		targetRotation = actor_->GetComponent<PhysicsBodyComponent>()->GetMaxRotation() * rotationSize / slowRadius;
	}
	// Final target rotation combines speed and directiontargetRotation *= rotation / rotationSize
	targetRotation *= rotation / rotationSize;

	// acceleration tries to get to target rotation
	result.angular = targetRotation;
	result.angular /= timeToTarget;
		
	// clip to max angular acceleration, if needed
	actor_->GetComponent<PhysicsBodyComponent>()->SetAngular(abs(result.angular));
	if (actor_->GetComponent<PhysicsBodyComponent>()->GetAngular() > actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration()) {
		result.angular /= actor_->GetComponent<PhysicsBodyComponent>()->GetAngular();
		result.angular *= actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration();
	}
	result.linear = Vec3();

	//std::cout << rotation << std::endl;

	return result;
}

/*Quaternion test = actor_->GetComponent<TransformComponent>()->GetQuaternion();
	float angleTest = (acos(test.w) * RADIANS_TO_DEGREES) * 2;
	std::cout << angleTest << std::endl;*/

double Align::mapToRange(double degrees) {

	double r = fmod(degrees, 360.0);

	if (r < -180.0)

		r += 360.0;

	if (r >= 180.0)

		r -= 360.0;

	return r;

}