#pragma once

#include "Actor.h"
#include "SteeringOutput.h"

//abstract class
//steering behaviour subclasses will inherit from here and
//must implement the getSteering function

class SteeringBehaviour {
protected:
	SteeringOutput result;
public:
	SteeringBehaviour();
	virtual SteeringOutput GetSteering(Ref<Actor> actor_) = 0;
	virtual bool OnCreate() = 0;
	virtual ~SteeringBehaviour();
};