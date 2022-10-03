#include "SteeringComponent.h"
#include "PhysicsBodyComponent.h"

#include "Seek.h"
#include "EngineManager.h"

SteeringComponent::SteeringComponent(Component* parent_, std::vector<Ref<SteeringBehaviour>> steeringBehaviours_, Ref<Actor> actor_) : Component(parent_) {
	steeringBehaviours = steeringBehaviours_;
	steering = SteeringOutput();
	actorComponent = actor_;
}

SteeringComponent::SteeringComponent(Component* parent_, Ref<SteeringBehaviour> steerBehaviour_, Ref<Actor> actor_) : Component(parent_) {
	steeringBehaviours.push_back(steerBehaviour_);
	steering = SteeringOutput();
	actorComponent = actor_;
}

SteeringComponent::~SteeringComponent() {
	OnDestroy();
}

bool SteeringComponent::OnCreate() {
	isCreated = true;
	return isCreated;
}

void SteeringComponent::OnDestroy() {
	steeringBehaviours.clear();
	isCreated = false;
}

void SteeringComponent::Update(const float deltaTime_) {
	// using the target, calculate and set values in the overall steering output

	RunSteering();

	// apply the steering to the equations of motion
	//just call the body update - then do all the steering update stuff in the steering component update
	if (actorComponent->GetComponent<PhysicsBodyComponent>() != nullptr) {
		//actorComponent->GetComponent<PhysicsBodyComponent>()->Update(deltaTime_);
		// Adjust velocity and rotation according to steering input
		actorComponent->GetComponent<PhysicsBodyComponent>()->SetAccel(steering.linear);
		actorComponent->GetComponent<TransformComponent>()->setOrientation(steering.rotation);

		//clip accel to max
		if (VMath::mag(actorComponent->GetComponent<PhysicsBodyComponent>()->GetAccel()) > actorComponent->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration()) {
			actorComponent->GetComponent<PhysicsBodyComponent>()->SetAccel(VMath::normalize(steering.linear) * actorComponent->GetComponent<PhysicsBodyComponent>()->GetMaxAcceleration());
		}

		//clip angular acceleration to max - angular = angular > maxAngular ? maxAngular : angular;
		/*if (actorComponent->GetComponent<PhysicsBodyComponent>()->GetAngular() > actorComponent->GetComponent<PhysicsBodyComponent>()->GetMaxAngular()) {
			actorComponent->GetComponent<PhysicsBodyComponent>()->SetAngular(actorComponent->GetComponent<PhysicsBodyComponent>()->GetMaxAngular());
		}*/
	}
}

void SteeringComponent::RunSteering() {
	steering = SteeringOutput();
	std::vector<SteeringOutput> steeringOutputs;
	for (unsigned i = 0; i < steeringBehaviours.size(); i++) {
		steeringOutputs.push_back(steeringBehaviours[i]->GetSteering(actorComponent));
	}

	//add together all steering outputs
	for (unsigned i = 0; i < steeringOutputs.size(); i++) {
		steering += steeringOutputs[i]; //use the overload in the steering output to add them together - * dereferences a pointer
	}
}

void SteeringComponent::Render() const {}
