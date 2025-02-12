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
	PhysicsBodyComponent(Component* parent_, 
		Ref<TransformComponent> transform_, 
		float mass_, 
		Vec3 vel_, 
		Vec3 accel_, 
		float maxSpeed_, 
		float maxAcceleration_, 
		Matrix3 rotationalInertia_, 
		Vec3 angularAccel_, 
		Vec3 angularVel_, 
		float maxAngular_);
	PhysicsBodyComponent(Component* parent_, float mass_, Vec3 vel_, Vec3 accel_, float maxSpeed_, float maxAcceleration_, Matrix3 rotationalInertia_, Vec3 angularAccel_, Vec3 angularVel_, float maxAngular_);

	virtual ~PhysicsBodyComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	void ApplyForce(MATH::Vec3 force_);

	MATH::Vec3 GetVel() { return vel; }
	MATH::Vec3 GetAccel() { return accel; }
	float GetMass() { return mass; }
	float GetMaxSpeed() { return maxSpeed; }
	float GetMaxAcceleration() { return maxAcceleration; }
	float GetMaxAngular() { return maxAngular; }
	MATH::Vec3 GetAngularVel() { return angularVel; }
	MATH::Vec3 GetAngularAccel() { return angularAccel; }
	Matrix3 GetRotationalInertia() { return rotationalInertia; }

	void SetVel(MATH::Vec3 vel_) { vel = vel_; }
	void SetAccel(MATH::Vec3 accel_) { accel = accel_; }
	void SetAngularVel(MATH::Vec3 angularVel_) { angularVel = angularVel_; }
	void SetAngularAccel(MATH::Vec3 angularAccel_) { angularAccel = angularAccel_; }

	MATH::Vec3 vel; //temp change just to make some stuff in the inputManager easier
private:
	Ref<TransformComponent> transform;
	float mass;
	// Linear
	MATH::Vec3 accel;
	//MATH::Vec3 vel;
	float maxSpeed;
	float maxAcceleration;
	// Angular
	Matrix3 rotationalInertia;
	Vec3 angularAccel;
	Vec3 angularVel;
	float maxAngular;
};

