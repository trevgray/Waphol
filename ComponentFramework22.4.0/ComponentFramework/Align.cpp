#include "Align.h"
#include "PhysicsBodyComponent.h"

Align::Align(Ref<TransformComponent> target_, Quaternion baseq_) {
	target = target_;
	baseq = baseq_;
}

Align::~Align() {

}

SteeringOutput Align::GetSteering(Ref<Actor> actor_) {
	Vec3 direction = target->GetPosition() - actor_->GetComponent<TransformComponent>()->GetPosition();
	float angleDirection = atan2(direction.y, direction.x) * RADIANS_TO_DEGREES;

	result.rotation = baseq * QMath::angleAxisRotation(angleDirection + 90, Vec3(0.0f,0.0f,1.0f));
	
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