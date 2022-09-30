#include "FaceVelocity.h"
#include "PhysicsBodyComponent.h"
#include "QMath.h"

FaceVelocity::FaceVelocity(Ref<Actor> target_, Quaternion baseQ_) {
	target = target_;
	baseQ = baseQ_;
}

FaceVelocity::~FaceVelocity() {

}

SteeringOutput FaceVelocity::GetSteering(Ref<Actor> actor_) {
	//Vec3 direction = target->GetComponent<TransformComponent>()->GetPosition() - target->GetComponent<PhysicsBodyComponent>()->GetVel();
	float angleDirection = atan2(target->GetComponent<PhysicsBodyComponent>()->GetVel().y, target->GetComponent<PhysicsBodyComponent>()->GetVel().x) * RADIANS_TO_DEGREES;

	result.rotation = baseQ * QMath::angleAxisRotation(angleDirection + 90, Vec3(0.0f, 0.0f, 1.0f));

	result.linear = Vec3();

	//std::cout << rotation << std::endl;

	return result;
}
