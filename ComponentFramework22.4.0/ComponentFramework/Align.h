#pragma once
#include "SteeringBehaviour.h"
#include "QMath.h"
class Align : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;

	// radius for arriving at the target
	float targetRadius;
	// radius for beginning to slow downs
	float slowRadius;
	// time over which to achieve target speed
	float timeToTarget = 0.1;
	//base orientation
	Quaternion baseq;
public:
	Align(Ref<TransformComponent> target_, Quaternion baseq_, float targetRadius_, float slowRadius_, float timeToTarget_);
	virtual ~Align();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
	double mapToRange(double degrees);
	float getLegAngle(Quaternion q);
};