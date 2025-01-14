#include "Sphere.h"
#include "MMath.h"
#include "Matrix.h"
#include "QuadraticSolution.h"

using namespace MATH;
using namespace GEOMETRY;

void Sphere::generateVerticesAndNormals()
{
	const float deltaTheta = 1.0f;
	const float deltaPhi = 360.0f / 15.0f;
	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta)
	{
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f);
		for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(deltaPhi, Vec3(0.0f, 1.0f, 0.0f));
			circle = rotationMatrix * circle;

			vertices.push_back(circle + Vec3(x,y,z));
			normals.push_back(circle);
		}
	}
}

RayIntersectionInfo GEOMETRY::Sphere::rayIntersectionInfo(const Ray& ray) const {
	RayIntersectionInfo rayInfo;
	const Vec3 sphereToRatStart = ray.start - Vec3(x, y, z); //from object space to "shape" space (moved the ray over to the shape)

	//Solve the quadratic function for this interaction
	//REFERENCE: Chapter 5 of Real Time Collision Detection by Ericson
	const float a = VMath::dot(ray.dir, ray.dir); //vector dot a vector returns the squared norm of the vector
	const float b = 2.0f * VMath::dot(sphereToRatStart, ray.dir);
	const float c = VMath::dot(sphereToRatStart, sphereToRatStart) - r * r;
	QuadraticSolution soln = QuadraticSolution::SolveQuadratic(a, b, c);
	//Gets the closer interaction point (Explained in Game Physics 3 Ray Sphere Collisions Slide 9)
	//The smaller t value is where the ray inters the sphere, and the larger t value is where the ray exits the sphere.
	if (soln.numSolutions == NumSolutions::zeroRoots) {
		rayInfo.isIntersected = false;
	}
	else if (soln.secondRoot < 0.0f) {
		rayInfo.isIntersected = true;
		//ray is going backwards
		//second root is the smallest absolute value
		rayInfo.intersectionPoint = ray.currentPosition(soln.secondRoot);
		rayInfo.t = soln.secondRoot;
	}
	else {
		rayInfo.isIntersected = true;
		rayInfo.intersectionPoint = ray.currentPosition(soln.firstRoot);
		rayInfo.t = soln.firstRoot;
	}
	return rayInfo;
}
