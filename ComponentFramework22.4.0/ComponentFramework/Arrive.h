#pragma once
#include "Seek.h"

class Arrive : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
	float targetRadius;
	float slowRadius;
	float timeToTarget;

	std::string targetName;
public:
	Arrive(Ref<TransformComponent> target_);
	Arrive(Ref<TransformComponent> target_, float targetRadius_, float slowRadius_, float timeToTarget);
	Arrive(std::string targetName_, float targetRadius_, float slowRadius_, float timeToTarget_);
	virtual ~Arrive();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

