#include "Align.h"
#include "PhysicsBodyComponent.h"
#include "EngineManager.h"

Align::Align(Ref<TransformComponent> target_, Quaternion baseq_) {
	target = target_;
	baseQ = baseq_;
	firstLoop = false;
}

Align::Align(std::string targetName_) {
	target = nullptr;
	baseQ = Quaternion();
	
	targetName = targetName_;
	firstLoop = false;
}

Align::~Align() {

}

bool Align::OnCreate() {
	//find parent actor
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second != nullptr && actor.first == targetName) {
			target = actor.second->GetComponent<TransformComponent>();
			break;
		}
	}
	return true;
}

SteeringOutput Align::GetSteering(Ref<Actor> actor_) {
	if (firstLoop == false) {
		baseQ = actor_->GetComponent<TransformComponent>()->GetQuaternion();
		firstLoop = true;
	}

	Vec3 direction = target->GetPosition() - actor_->GetComponent<TransformComponent>()->GetPosition();
	float angleDirection = atan2(direction.y, direction.x) * RADIANS_TO_DEGREES;

	result.rotation = baseQ * QMath::angleAxisRotation(angleDirection + 90, Vec3(0.0f,0.0f,1.0f));
	
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