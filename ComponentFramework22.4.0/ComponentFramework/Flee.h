#pragma once
#include "SteeringBehaviour.h"

class Flee : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
public:
	Flee(Ref<TransformComponent> target_);
	virtual ~Flee();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

