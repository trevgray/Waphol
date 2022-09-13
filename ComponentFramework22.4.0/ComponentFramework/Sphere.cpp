#include "Sphere.h"
#include "MMath.h"
#include "Matrix.h"

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
			vertices.push_back(circle);
			normals.push_back(circle - Vec3(x, y, z));
		}
	}
}