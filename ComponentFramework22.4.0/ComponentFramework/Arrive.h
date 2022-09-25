#pragma once
#include "Seek.h"

class Arrive : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
	float targetRadius;
	float slowRadius;
	float timeToTarget;
public:
	Arrive(Ref<TransformComponent> target_);
	Arrive(Ref<TransformComponent> target_, float targetRadius_, float slowRadius_, float timeToTarget);
	virtual ~Arrive();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

