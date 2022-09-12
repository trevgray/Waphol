#pragma once
#include "Component.h"
#include "Matrix.h"
#include "Actor.h"
using namespace MATH;
class MouseMoveableComponent : public Component {
private:
	Matrix4 invNDC;			/// the inverse of the viewportNDC matrix
	Vec3 lastMousePos;
public:
	MouseMoveableComponent(Component* parent_);
	~MouseMoveableComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;

	void setWindowDimensions();

	Vec3 getMouseVector(int x, int y, Actor *actor, Matrix4 projectionMatrix);

	void SetMousePos2(int x, int y);


};

