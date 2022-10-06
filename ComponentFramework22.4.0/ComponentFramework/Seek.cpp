#include "Seek.h"
#include "PhysicsBodyComponent.h"
#include "EngineManager.h"

Seek::Seek(Ref<TransformComponent> target_) {
	target = target_;
}

Seek::Seek(std::string targetName_) {
	targetName = targetName_;
	target = nullptr;
}

Seek::~Seek() {

}

bool Seek::OnCreate() {
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second != nullptr && actor.first == targetName) {
			target = actor.second->GetComponent<TransformComponent>();
			break;
		}
	}
	return true;
}

SteeringOutput Seek::GetSteering(Ref<Actor> actor_) {
	//get direction to target
	result.linear = target->GetPosition() - actor_->GetComponent<TransformComponent>()->GetPosition();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	//accelerate along this direction
	result.linear = VMath::normalize(result.linear) * actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	result.rotation = Quaternion(-1, Vec3(0.0f, 0.0f, 0.0f));

	return result;
}