#include "FaceVelocity.h"
#include "PhysicsBodyComponent.h"
#include "QMath.h"
#include "EngineManager.h"

FaceVelocity::FaceVelocity(Ref<Actor> target_, Quaternion baseQ_) {
	target = target_;
	baseQ = baseQ_;
}

FaceVelocity::FaceVelocity(std::string targetName_) {
	target = nullptr;
	baseQ = Quaternion();

	targetName = targetName_;
	firstLoop = false;
}

FaceVelocity::~FaceVelocity() {

}

bool FaceVelocity::OnCreate() {
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second != nullptr && actor.first == targetName) {
			target = actor.second;
			break;
		}
	}
	return true;
}

SteeringOutput FaceVelocity::GetSteering(Ref<Actor> actor_) {
	if (firstLoop == false) {
		baseQ = actor_->GetComponent<TransformComponent>()->GetQuaternion();
		firstLoop = true;
	}

	//Vec3 direction = target->GetComponent<TransformComponent>()->GetPosition() - target->GetComponent<PhysicsBodyComponent>()->GetVel();
	float angleDirection = atan2(target->GetComponent<PhysicsBodyComponent>()->GetVel().y, target->GetComponent<PhysicsBodyComponent>()->GetVel().x) * RADIANS_TO_DEGREES;

	result.rotation = baseQ * QMath::angleAxisRotation(angleDirection + 90, Vec3(0.0f, 0.0f, 1.0f));

	result.linear = Vec3();

	//std::cout << rotation << std::endl;

	return result;
}
