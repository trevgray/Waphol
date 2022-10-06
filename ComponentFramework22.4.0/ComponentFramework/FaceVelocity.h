#pragma once
#include "SteeringBehaviour.h"
class FaceVelocity : public SteeringBehaviour {
protected:
	Ref<Actor> target;
	//base orientation
	Quaternion baseQ;

	std::string targetName;
	bool firstLoop;
public:
	FaceVelocity(Ref<Actor> target_, Quaternion baseQ_);
	FaceVelocity(std::string targetName_);
	virtual ~FaceVelocity();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
};

