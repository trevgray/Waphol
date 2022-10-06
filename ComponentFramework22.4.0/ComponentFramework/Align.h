#pragma once
#include "SteeringBehaviour.h"
#include "QMath.h"
class Align : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
	//base orientation
	Quaternion baseQ;

	std::string targetName;
	bool firstLoop;
public:
	Align(Ref<TransformComponent> target_, Quaternion baseq_);
	Align(std::string targetName_);
	virtual ~Align();
	bool OnCreate();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
	double mapToRange(double degrees);
};