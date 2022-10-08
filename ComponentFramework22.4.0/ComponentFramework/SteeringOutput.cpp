#include "SteeringOutput.h"
#include "Quaternion.h"

SteeringOutput::SteeringOutput() {
	linear = Vec3();
	rotation = Quaternion();
	weight = 1.0f;
}

SteeringOutput::SteeringOutput(Vec3 linear_, Quaternion rotation_) {
	linear = linear_;
	rotation = rotation_;
	weight = 1.0f;
}

SteeringOutput::~SteeringOutput() {

}