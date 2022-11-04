#include "SteeringOutput.h"
#include "Quaternion.h"

SteeringOutput::SteeringOutput() {
	linear = Vec3();
	rotation = Quaternion(-1, Vec3(0.0f, 0.0f, 0.0f));
	weight = 1.0f;
}

SteeringOutput::SteeringOutput(Vec3 linear_, Quaternion rotation_) {
	linear = linear_;
	rotation = rotation_;
	weight = 1.0f;
}

SteeringOutput::~SteeringOutput() {

}