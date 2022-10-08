#include "SteeringBehaviour.h"
#include "Debug.h"

SteeringBehaviour::SteeringBehaviour() {
	result = SteeringOutput();
}

//SteeringOutput SteeringBehaviour::GetSteering(Ref<Actor> actor_) { //define this function so your can instantiate the class - this should never be called or there is a problem
//#ifdef _DEBUG
//	Debug::Error("Error: Undefined Steering Behaviour", __FILE__, __LINE__);
//#endif
//	return SteeringOutput();
//}

SteeringBehaviour::~SteeringBehaviour() {}

void SteeringBehaviour::SetWeight(float weight_) {
	result.weight = weight_;
}