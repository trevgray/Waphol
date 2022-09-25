#pragma once
#include "SteeringBehaviour.h"
class VelocityMatch : public SteeringBehaviour {
protected:
	Ref<Actor> target;
	float timeToTarget;
public:
	VelocityMatch(Ref<Actor> target_, float timeToTarget_);
	virtual ~VelocityMatch();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

