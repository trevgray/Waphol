#include "Flee.h"
#include "PhysicsBodyComponent.h"

Flee::Flee(Ref<TransformComponent> target_) {
	target = target_;
}

Flee::~Flee() {

}

bool Flee::OnCreate() {
	return true;
}

SteeringOutput Flee::GetSteering(Ref<Actor> actor_) {
	//get direction to target
	result.linear = actor_->GetComponent<TransformComponent>()->GetPosition() - target->GetPosition();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	//accelerate along this direction
	result.linear = VMath::normalize(result.linear) * actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	result.rotation = Quaternion();

	return result;
}
