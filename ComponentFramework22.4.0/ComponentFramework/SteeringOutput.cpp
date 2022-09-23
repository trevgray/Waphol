#include "SteeringOutput.h"

SteeringOutput::SteeringOutput() {
	linear = Vec3();
	angular = 0.0f;
}

SteeringOutput::SteeringOutput(Vec3 linear_, float angular_) {
	linear = linear_;
	angular = angular_;
}

SteeringOutput::~SteeringOutput() {

}