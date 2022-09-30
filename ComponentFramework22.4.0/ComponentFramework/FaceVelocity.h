#pragma once
#include "SteeringBehaviour.h"
class FaceVelocity : public SteeringBehaviour {
protected:
	Ref<Actor> target;
	//base orientation
	Quaternion baseQ;
public:
	FaceVelocity(Ref<Actor> target_, Quaternion baseQ_);
	virtual ~FaceVelocity();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

