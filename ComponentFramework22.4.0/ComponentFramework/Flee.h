#pragma once
#include "SteeringBehaviour.h"

class Flee : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
public:
	Flee(Ref<TransformComponent> target_);
	virtual ~Flee();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

