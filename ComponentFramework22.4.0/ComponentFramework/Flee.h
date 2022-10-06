#pragma once
#include "SteeringBehaviour.h"

class Flee : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
	std::string targetName;
public:
	Flee(Ref<TransformComponent> target_);
	Flee(std::string targetName_);
	virtual ~Flee();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

