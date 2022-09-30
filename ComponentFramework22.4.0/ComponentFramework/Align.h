#pragma once
#include "SteeringBehaviour.h"
#include "QMath.h"
class Align : public SteeringBehaviour {
protected:
	Ref<TransformComponent> target;
	//base orientation
	Quaternion baseq;
public:
	Align(Ref<TransformComponent> target_, Quaternion baseq_);
	virtual ~Align();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);
	double mapToRange(double degrees);
};