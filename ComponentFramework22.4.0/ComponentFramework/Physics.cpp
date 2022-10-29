#include "Physics.h"
#include "VMath.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Matrix.h"

#include "PhysicsBodyComponent.h"
#include "MMath.h"
#include "ShapeComponent.h"
#include "EngineManager.h"

void Physics::SimpleNewtonMotion(Ref<Actor> object, const float deltaTime) {
	Ref<TransformComponent> actorTransform = object->GetComponent<TransformComponent>();
	Ref<PhysicsBodyComponent> actorBody = object->GetComponent<PhysicsBodyComponent>();

	//object.pos += object.vel * deltaTime + 0.5f * object.accel * deltaTime * deltaTime;
	actorTransform->SetPosition(actorTransform->GetPosition() + actorBody->GetVel() * deltaTime + 0.5f * actorBody->GetAccel() * deltaTime * deltaTime);
	//object.vel += object.accel * deltaTime;
	actorBody->SetVel(actorBody->GetVel() + actorBody->GetAccel() * deltaTime);
}

void Physics::RigidBodyRotation(Ref<Actor> object, const float deltaTime) { //I use quaternions so
	//object.angle += object.angularVel * deltaTime + 0.5f * object.angularAccel * deltaTime * deltaTime;
	//object.angularVel += object.angularAccel * deltaTime;
}

Vec3 Physics::RotateZ(float angleDisplacement, Vec3 force)
{
	Matrix2 rotateMatrix;
	//Converting to radians from the displacement
	float rads = (angleDisplacement * M_PI) / 180;
	//2D transformation matrix
	rotateMatrix.m[0] = cos(rads);
	rotateMatrix.m[1] = sin(rads);
	rotateMatrix.m[2] = sin(rads);
	rotateMatrix.m[3] = cos(rads);
	//dot product the rotate matrix to the force matrix
	float x = (rotateMatrix.m[0] * force.x) - (rotateMatrix.m[1] * force.y);
	float y = (rotateMatrix.m[2] * force.x) + (rotateMatrix.m[3] * force.y);

	return Vec3(x, y, force.z);
}

bool Physics::PlaneSphereCollision(Ref<Actor> object, Plane &p) {
	/*float distance = p.distance(object);
	if (distance <= object.sphere.r) {
		return true;
	}
	else {
		return false;
	}*/

	return true;
}

void Physics::PlaneSphereCollisionResponse(Ref<Actor> object, Plane &p) {
	//if (PlaneSphereCollision(object,p)) {
	//	//Vec3 normal = Vec3(p.y, -p.x, p.z);
	//	Vec3 normal = Vec3(p.x, p.y, p.z);
	//	normal.Normalize();
	//	Vec3 projectionVector = (-object.vel).Dot(normal) * normal;
	//	object.vel = object.vel + (2 * projectionVector);
	//}
}

bool Physics::SphereSphereCollision(const Ref<Actor> object1, const Ref<Actor> object2) {
	/*Vec3 dir = object1.pos - object2.pos;
	float mag = dir.Mag();
	if (mag < (object1.sphere.r + object2.sphere.r)) {
		return true;
	}
	else {
		return false;
	}*/


	/*
	float dist = sqrt(pow(2, (object2.pos.x - object1.pos.x)) + pow(2, (object2.pos.y - object1.pos.y)) + pow(2, (object2.pos.z - object1.pos.z)));
	if (dist < (object2.sphere.r + object1.sphere.r)) {
		return true;
	}
	else {
		return false;
	}*/

	return true;
}

void Physics::SphereSphereCollisionResponse(Ref<Actor> object1, Ref<Actor> object2, float CR) {
	if (SphereSphereCollision(object1, object2)) {
		//find normal and normalize it
		
		//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
		//Vec3 projectionVector1 = (-object1.vel).Dot(normal) * normal;
		//find the projection mag
		
		//std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;
		//Vec3 projectionVector2 = (-object2.vel).Dot(normal) * normal;
		
		//------------------------------

		////velocities in the direction of the normal
		//Vec3 normal;
		//normal = object1.pos - object2.pos;
		//normal.Normalize();
		//float projMag1 = -object1.vel.Dot(normal);

		//normal = object2.pos - object1.pos;
		//normal.Normalize();
		//float projMag2 = -object2.vel.Dot(normal);
		////Conservation of Momentum equation substituted into Coefficient of Restitution equation
		//float CR1 = ((object1.mass - CR * object2.mass) * (1 + CR) * object2.mass * projMag2) / (object1.mass + object2.mass);
		//float CR2 = ((object2.mass - CR * object1.mass) * (1 + CR) * object1.mass * projMag1) / (object1.mass + object2.mass);
		////apply the projection vector on both objects
		//object1.vel = (object1.vel + (CR1 - projMag1) * normal);
		//object2.vel = (object2.vel + (CR2 - projMag2) * normal);

		//------------------------------

		/*Vec3 finalVel1 = ((object1.mass * object1.vel) + (object2.mass * object2.vel) + (object1.mass * CR) * (object2.vel - object1.vel) / (object1.mass + object2.mass));
		Vec3 finalVel2 = ((object1.mass * object1.vel) + (object2.mass * object2.vel) + (object1.mass * CR) * (object1.vel - object2.vel) / (object1.mass + object2.mass));
		object1.setVel(finalVel1);
		object2.setVel(finalVel2);*/
		//object1.vel = object1.vel + (2 * projectionVector1);
		//object2.vel = object2.vel + (2 * projectionVector2);
	}
}

void Physics::SphereStaticSphereCollisionResponse(Ref<Actor> object1, Ref<Actor> object2, float CR) {
	//if (SphereSphereCollision(object1, object2)) {
	//	Vec3 normal;
	//	Vec3 projectionVector;
	//	normal = object1.pos - object2.pos;
	//	normal.Normalize();
	//	projectionVector = (-object1.vel).Dot(normal) * normal;
	//	//Vec3 projectionVector2 = (-object2.vel).Dot(normal) * normal;
	//	//float projMag1 = -object1.vel.Dot(normal);
	//	//normal = object2.pos - object1.pos;
	//	//normal.Normalize();
	//	//float projMag2 = -object2.vel.Dot(normal);
	//	//float CR1 = ((object1.mass - CR * object2.mass) * (1 + CR) * object2.mass * projMag2) / (object1.mass * object2.mass);
	//	//object1.vel = (object1.vel + (CR1 - projMag1) * normal);
	//	object1.vel = object1.vel + (2 * projectionVector);
	//}
}

void Physics::BuoyancyApplyForces(Ref<Actor> object, float waterHeight, float dragCoefficient, float liquidDensity, float length) {
	//float gravitationalAcceleration = 9.81f;
	//Vec3 weightForce = object.mass * Vec3(0.0f, -gravitationalAcceleration, 0.0f);
	//Vec3 dragForce = -dragCoefficient * object.getVel();
	////Vec3 dragForce;
	//float mediumDensity = liquidDensity;
	//float volume = 0.0f; //default volume
	//if ((object.pos.y + object.sphere.r) <= waterHeight) { //object under fully
	//	volume = M_PI * object.sphere.r * object.sphere.r * length;
	//	mediumDensity = liquidDensity;
	//	printf("Under Full Amount\n");
	//}
	//else if ((object.pos.y - object.sphere.r) <= waterHeight && object.pos.y >= waterHeight) { //object is under the water a small amount
	//	float disToWater = abs(object.pos.y - waterHeight);
	//	float theta = 2 * acos(std::clamp(disToWater / object.sphere.r, -1.0f, 1.0f));
	//	mediumDensity = liquidDensity;
	//	volume = 0.5 * object.sphere.r * object.sphere.r * (theta - sin(theta)) * length;
	//	printf("Under Small Amount\n");
	//}
	//else if ((object.pos.y) < waterHeight) { //the object is under the water a large amount
	//	float disToWater = abs(object.pos.y - waterHeight);
	//	float theta = 2 * acos(std::clamp(disToWater / object.sphere.r, -1.0f, 1.0f));
	//	volume = (M_PI * object.sphere.r * object.sphere.r * length) - (0.5 * object.sphere.r * object.sphere.r) * (theta - sin(theta)) * length;
	//	mediumDensity = liquidDensity;
	//	printf("Under Large Amount\n");
	//}
	//else if (waterHeight < object.pos.y - object.sphere.r) { //Air
	//	//volume = 0.0f;
	//	//mediumDensity = 1.225f;
	//	printf("In Air\n");
	//}
	//else {
	//	printf("ERROR\n");
	//}
	//Vec3 buoyancyForce = mediumDensity * Vec3(0.0f, gravitationalAcceleration, 0.0f) * volume;
	//object.accel = ((weightForce + dragForce + buoyancyForce)/ object.mass);
}

void Physics::VerletMotion(Ref<Actor> object, const float deltaTime, Vec3 previousAccel) {
	//object.pos += object.vel * deltaTime + (0.5f * previousAccel * deltaTime * deltaTime);
	//object.vel += 0.3f * (previousAccel + object.accel) * deltaTime; //0.3f
}

Hit Physics::LineTrace(GEOMETRY::Ray rayWorldSpace) {
	Hit hitResult;

	//Loop through all the actors and check if the ray has collided with them
	//Pick the one with the smallest positive t value
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second->GetComponent<ShapeComponent>() != nullptr) {
			Ref<TransformComponent> transformComponent = actor.second->GetComponent<TransformComponent>();
			Ref<ShapeComponent> shapeComponent = actor.second->GetComponent<ShapeComponent>();
			//Transform the ray into the local space of the object and check if a collision occurred
			Vec3 rayStartInObjectSpace = MMath::inverse(actor.second->GetModelMatrix()) * rayWorldSpace.start;
			Vec3 rayDirInObjectSpace = MMath::inverse(actor.second->GetModelMatrix()).multiplyWithoutDividingOutW(Vec4(rayWorldSpace.dir, 0.0f));

			GEOMETRY::Ray rayInObjectSpace{ rayStartInObjectSpace, rayDirInObjectSpace };
			//std::cout << "Checking: " << actor.first << '\n';
			GEOMETRY::RayIntersectionInfo rayInfo = shapeComponent->shape->rayIntersectionInfo(rayInObjectSpace);

			if (rayInfo.isIntersected) {
				hitResult.hitActor = actor.second;
				hitResult.intersectionPoint = rayInfo.intersectionPoint;
				hitResult.hitActorName = actor.first;
				hitResult.isIntersected = true;
			}
		}
	}
	return hitResult;
}