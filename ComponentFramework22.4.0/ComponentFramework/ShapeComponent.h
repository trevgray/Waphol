#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H

#include "Component.h"
#include "Shape.h"
#include "Sphere.h"

enum class ShapeType {
	sphere,
	cylinder,
	capsule,
	box
};

class ShapeComponent: public Component{
	// Scott hates these implicit copy/moves
	ShapeComponent(const ShapeComponent&) = delete;
	ShapeComponent(ShapeComponent&&) = delete;
	ShapeComponent& operator = (const ShapeComponent&) = delete;
	ShapeComponent& operator = (ShapeComponent&&) = delete;

public:
	ShapeComponent(Component* parent_, GEOMETRY::Sphere);
	//ShapeComponent(Component*, GEOMETRY::Cylinder); (TODO)
	//ShapeComponent(Component*, GEOMETRY::Capsule);  (TODO)
	//ShapeComponent(Component*, GEOMETRY::Box); (TODO)

	Ref<GEOMETRY::Shape> shape;
	ShapeType shapeType;
	virtual ~ShapeComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;
};

#endif
