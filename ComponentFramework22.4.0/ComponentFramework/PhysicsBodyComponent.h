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
		float radius_,
		float orientation_,
		float rotation_,
		float angular_,
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

	virtual MATH::Vec3 getVel() { return vel; }
	virtual MATH::Vec3 getAccel() { return accel; }
	virtual float getMass() { return mass; }
	virtual float getOrientation() { return orientation; }
	virtual float getRotation() { return rotation; }
	virtual float getAngular() { return angular; }
	virtual float getMaxSpeed() { return maxSpeed; }
	virtual float getMaxAcceleration() { return maxAcceleration; }
	virtual float getMaxRotation() { return maxRotation; }
	virtual float getMaxAngular() { return maxAngular; }

	virtual void setVel(MATH::Vec3 vel_) { vel = vel_; }
private:
	Ref<TransformComponent> transform;

	MATH::Vec3 vel;
	MATH::Vec3 accel;
	float mass;
	float orientation;	// facing this direction
	float rotation;			// rotating at this speed (radians per second?)
	float angular;          // angular acceleration
	float maxSpeed;
	float maxAcceleration;
	float maxRotation;
	float maxAngular;
	float radius;           // for getting near walls
};

