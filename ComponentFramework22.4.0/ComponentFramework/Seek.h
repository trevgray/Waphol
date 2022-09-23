#pragma once
#include "SteeringBehaviour.h"

class Seek : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
public:
	Seek(Ref<TransformComponent> target_);
	virtual ~Seek();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

