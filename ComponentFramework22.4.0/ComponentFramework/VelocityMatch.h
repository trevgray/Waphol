#pragma once
#include "SteeringBehaviour.h"
class VelocityMatch : public SteeringBehaviour {
protected:
	Ref<Actor> target;
	float timeToTarget;
	std::string targetName;
public:
	VelocityMatch(Ref<Actor> target_, float timeToTarget_);
	VelocityMatch(std::string targetName_, float timeToTarget_);
	virtual ~VelocityMatch();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

