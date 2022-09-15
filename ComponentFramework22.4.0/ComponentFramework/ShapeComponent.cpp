#include "ShapeComponent.h"

ShapeComponent::ShapeComponent(Component* parent_, GEOMETRY::Sphere sphere_) :Component(parent_)
{
	shapeType = ShapeType::sphere;
	shape = std::make_shared<GEOMETRY::Sphere>(MATH::Vec3(sphere_.x, sphere_.y, sphere_.z), sphere_.r);
}

ShapeComponent::ShapeComponent(Component* parent_, GEOMETRY::Cylinder cylinder_) :Component(parent_)
{
	shapeType = ShapeType::cylinder;
	shape = std::make_shared<GEOMETRY::Cylinder>(cylinder_.r, cylinder_.capCentrePosA, cylinder_.capCentrePosB);
}

ShapeComponent::~ShapeComponent()
{
}

bool ShapeComponent::OnCreate()
{
	return true;
}

void ShapeComponent::OnDestroy()
{
}

void ShapeComponent::Update(const float deltaTime_)
{
}

void ShapeComponent::Render() const
{
	shape->debugDraw();
}
