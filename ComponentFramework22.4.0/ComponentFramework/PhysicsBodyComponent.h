#pragma once
#include "Component.h"
#include "TransformComponent.h"
#include "Vector.h"

class PhysicsBodyComponent : public Component {
	PhysicsBodyComponent(const PhysicsBodyComponent&) = delete;
	PhysicsBodyComponent(PhysicsBodyComponent&&) = delete;
	PhysicsBodyComponent& operator = (const PhysicsBodyComponent&) = delete;
	PhysicsBodyComponent& operator = (PhysicsBodyComponent&&) = delete;
public:
	PhysicsBodyComponent(Component* parent_, Ref<TransformComponent> transform_);
	PhysicsBodyComponent(Component* parent_, Ref<TransformComponent> transform_, MATH::Vec3 vel_, MATH::Vec3 accel_,
		float mass_,
		float maxSpeed_,
		float maxAcceleration_,
		float maxRotation_,
		float maxAngular_);
	PhysicsBodyComponent(Component* parent_, MATH::Vec3 vel_, MATH::Vec3 accel_,
		float mass_,
		float maxSpeed_,
		float maxAcceleration_,
		float maxRotation_,
		float maxAngular_);

	virtual ~PhysicsBodyComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	virtual void ApplyForce(MATH::Vec3 force_);

	virtual MATH::Vec3 GetVel() { return vel; }
	virtual MATH::Vec3 GetAccel() { return accel; }
	virtual float GetMass() { return mass; }
	virtual float GetMaxSpeed() { return maxSpeed; }
	virtual float GetMaxAcceleration() { return maxAcceleration; }
	virtual float GetMaxAngular() { return maxAngular; }

	virtual void SetVel(MATH::Vec3 vel_) { vel = vel_; }
	virtual void SetAccel(MATH::Vec3 accel_) { accel = accel_; }

	MATH::Vec3 vel;
private:
	Ref<TransformComponent> transform;

	MATH::Vec3 accel;
	float mass;
	float maxSpeed;
	float maxAcceleration;
	float maxAngular;
};

