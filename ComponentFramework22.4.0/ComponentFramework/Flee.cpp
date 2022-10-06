#include "Flee.h"
#include "PhysicsBodyComponent.h"
#include "EngineManager.h"

Flee::Flee(Ref<TransformComponent> target_) {
	target = target_;
}

Flee::Flee(std::string targetName_) {
	targetName = targetName_;
	target = nullptr;
}

Flee::~Flee() {

}

bool Flee::OnCreate() {
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second != nullptr && actor.first == targetName) {
			target = actor.second->GetComponent<TransformComponent>();
			break;
		}
	}
	return true;
}

SteeringOutput Flee::GetSteering(Ref<Actor> actor_) {
	//get direction to target
	result.linear = actor_->GetComponent<TransformComponent>()->GetPosition() - target->GetPosition();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	//accelerate along this direction
	result.linear = VMath::normalize(result.linear) * actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	result.rotation = Quaternion(-1, Vec3(0.0f, 0.0f, 0.0f));

	return result;
}
