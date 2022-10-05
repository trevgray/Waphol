#pragma once
#include "SteeringBehaviour.h"

class Seek : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
public:
	Seek(Ref<TransformComponent> target_);
	virtual ~Seek();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

