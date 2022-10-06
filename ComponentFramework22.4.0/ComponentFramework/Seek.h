#pragma once
#include "SteeringBehaviour.h"

class Seek : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
	std::string targetName;
public:
	Seek(Ref<TransformComponent> target_);
	Seek(std::string targetName_);
	virtual ~Seek();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

