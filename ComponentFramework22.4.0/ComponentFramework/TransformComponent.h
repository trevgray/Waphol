#pragma once
#include "Component.h"
#include "Matrix.h"
#include "Quaternion.h"
using namespace MATH;
class TransformComponent : public Component {
private:
	Vec3 pos;
	Quaternion orientation;
	Vec3 scale;
public:
	TransformComponent(Component* parent_);
	TransformComponent(Component* parent_, Vec3 pos_, Quaternion orientation_, Vec3 scale_ = Vec3(1.0f,1.0f,1.0f));
	~TransformComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;

	inline Vec3 GetPosition() { return pos; }
	inline Quaternion GetQuaternion() { return orientation; }
	Matrix4 GetTransformMatrix() const;
	void SetTransform(Vec3 pos_, Quaternion orientation_, Vec3 scale_ = Vec3(1.0f, 1.0f, 1.0f)) {
		pos - pos_;
		orientation = orientation_;
		scale = scale_;
	}
	void setRotation(Quaternion orientation_) {
		orientation = orientation_;
	}
	void SetPosition(Vec3 pos_) {
		pos = pos_;
	}
};

