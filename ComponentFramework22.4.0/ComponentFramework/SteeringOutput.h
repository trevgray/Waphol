#pragma once

#include "Vector.h"
#include "Quaternion.h"
using namespace MATH;

class SteeringOutput
{
public:
	// Yes these are public! 
	// The steering algorithms will be creating instances and then setting the values.
	// There might be a better way, but I can't think of it right now.

	Vec3 linear;
	Quaternion rotation;

	SteeringOutput();
	SteeringOutput(Vec3 linear_, Quaternion rotation_);
	~SteeringOutput();

	inline const SteeringOutput operator + (const SteeringOutput& v) const {
		return SteeringOutput(linear + v.linear, rotation + v.rotation);
	}

	inline SteeringOutput& operator += (const SteeringOutput& v) {
		linear += v.linear;
		rotation = v.rotation;
		return *this;
	}


	// This class does not have any behaviours, just constructors!

};