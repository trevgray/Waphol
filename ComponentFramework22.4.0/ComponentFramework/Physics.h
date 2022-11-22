#ifndef PHYSICS_H
#define PHYSICS_H
#include "Actor.h"
#include "Plane.h"
#include "Ray.h"
#include <iostream>

struct Hit {
	bool isIntersected;
	Vec3 intersectionPoint;
	Ref<Actor> hitActor;
	std::string hitActorName;

	Hit() {
		isIntersected = false;
		intersectionPoint = Vec3();
	}
};

class Physics {
public:
	static void SimpleNewtonMotion(Ref<Actor> object, const float deltaTime);
	static void RigidBodyRotation(Ref<Actor> object, const float deltaTime);
	//Updates angular acceleration using torque/rotation inertia
	static void ApplyTorque(Ref<Actor> object, Vec3 torque);
	//Updates acceleration using a = F/m
	static void ApplyForce(Ref<Actor> object, Vec3 force);

	static Vec3 RotateZ(float angleDisplacement, Vec3 force);
	static bool PlaneSphereCollision(Ref<Actor> object, Plane &p);
	static void PlaneSphereCollisionResponse(Ref<Actor> object, Plane &p);
	static bool SphereSphereCollision(const Ref<Actor> object1, const Ref<Actor> object2);
	static void SphereSphereCollisionResponse(Ref<Actor> object1, Ref<Actor> object2, float CR);
	static void SphereStaticSphereCollisionResponse(Ref<Actor> object1, Ref<Actor> object2, float CR);
	static void BuoyancyApplyForces(Ref<Actor> object, float waterHeight, float dragCoefficient, float liquidDensity, float length);
	static void VerletMotion(Ref<Actor> object, const float deltaTime, Vec3 previousAccel);

	static Hit LineTrace(GEOMETRY::Ray rayWorldSpace);
};

#endif