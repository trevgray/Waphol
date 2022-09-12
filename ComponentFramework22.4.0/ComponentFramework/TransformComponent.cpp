//#include <iostream>
#include "TransformComponent.h"
#include "QMath.h"
#include "MMath.h"
using namespace MATH;
TransformComponent::TransformComponent(Component* parent_):Component(parent_), scale(1.0f,1.0f,1.0f), 
pos(0.0f, 0.0f, 0.0f), orientation(Quaternion(1.0f, Vec3(0.0f,0.0f,0.0f))) {}

TransformComponent::TransformComponent(Component* parent_, Vec3 pos_, Quaternion orientation_, Vec3 scale_): Component(parent_), scale(scale_),
pos(pos_), orientation(Quaternion(orientation_)) {
	//Component*(parent) = parent_; just seeing how to initialize Component in the {}
}

TransformComponent::~TransformComponent() {
	OnDestroy();
}
bool TransformComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}
void TransformComponent::OnDestroy() {
	isCreated = false;
}

void TransformComponent::Update(const float deltaTime) {

}
void TransformComponent::Render()const {}

Matrix4 TransformComponent::GetTransformMatrix() const {
	return MMath::translate(pos) * MMath::scale(scale) * QMath::toMatrix4(orientation);
	// read matrices <------- this way from the view of models
	// read matrices -------> this way from the view of world
}
