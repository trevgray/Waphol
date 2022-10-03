#include "Seek.h"
#include "PhysicsBodyComponent.h"

Seek::Seek(Ref<TransformComponent> target_) {
	target = target_;
}

Seek::~Seek() {

}

SteeringOutput Seek::GetSteering(Ref<Actor> actor_) {
	//get direction to target
	result.linear = target->GetPosition() - actor_->GetComponent<TransformComponent>()->GetPosition();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	//accelerate along this direction
	result.linear = VMath::normalize(result.linear) * actor_->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration();

	//std::cout << result.linear.x << " " << result.linear.y << " " << result.linear.z << std::endl;

	result.rotation = Quaternion();

	return result;
}