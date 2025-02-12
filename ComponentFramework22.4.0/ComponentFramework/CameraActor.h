#pragma once
#include "Actor.h"
#include "Matrix.h"
#include "Ray.h"

#include "Plane.h"
#include <array>

using namespace MATH;
class CameraActor : public Actor {
private:
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	unsigned int uboMatricesID; //matrixID
	const unsigned int bindingPoint = 0; //must be unique to the camera - so = 0, lights will be = 1

	//Frustum Culling
	//have the camera hold all 6 planes
	std::array<Plane, 6> frustumPlanes;

	void CalculateFrustumPlanes();

	//then check against some point (lets start with the center point of the actor to start), in PMath check the distance to the point
	//if the point is negative, then the point is outside - otherwise the object is in the field of view
public:
	CameraActor(Component* parent_);
	virtual ~CameraActor();

	bool OnCreate() override;
	void OnDestroy() override;

	Matrix4 GetProjectionMatrix() const { return projectionMatrix; }
	Matrix4 GetViewMatrix() const { return viewMatrix; }

	std::array<Plane, 6> GetFrustumPlanes() { return frustumPlanes; }

	void UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far);
	void UpdateViewMatrix();

	unsigned int GetMatriciesID() const { return uboMatricesID; }

	GEOMETRY::Ray WorldSpaceRayFromMouseCoords(float mouseX, float mouseY);
};