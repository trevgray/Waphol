#include "Box.h"
#include "QMath.h"

void GEOMETRY::Box::generateVerticesAndNormals() {
	//vertex points
	//bottomRightBack = orientation * (center + MATH::Vec3(halfExtent.x, -halfExtent.y, -halfExtent.z)) //also works
	MATH::Vec3 bottomRightBack = MATH::QMath::rotate(center + MATH::Vec3(halfExtent.x, -halfExtent.y, -halfExtent.z), orientation);
	MATH::Vec3 bottomLeftBack = MATH::QMath::rotate(center + MATH::Vec3(-halfExtent.x, -halfExtent.y, -halfExtent.z), orientation);

	MATH::Vec3 bottomLeftFront = MATH::QMath::rotate(center + MATH::Vec3(-halfExtent.x, -halfExtent.y, halfExtent.z), orientation);
	MATH::Vec3 bottomRightFront = MATH::QMath::rotate(center + MATH::Vec3(halfExtent.x, -halfExtent.y, halfExtent.z), orientation);

	MATH::Vec3 topRightBack = MATH::QMath::rotate(center + MATH::Vec3(halfExtent.x, halfExtent.y, -halfExtent.z), orientation);
	MATH::Vec3 topLeftBack = MATH::QMath::rotate(center + MATH::Vec3(-halfExtent.x, halfExtent.y, -halfExtent.z), orientation);

	MATH::Vec3 topRightFront = MATH::QMath::rotate(center + halfExtent, orientation);
	MATH::Vec3 topLeftFront = MATH::QMath::rotate(center + MATH::Vec3(-halfExtent.x, halfExtent.y, halfExtent.z), orientation);

	//bottom
	//face 1
	//bottom right back x,y,z vertex
	vertices.push_back(bottomRightBack);
	normals.push_back(bottomRightBack);
	//bottom right front x,y,z vertex
	vertices.push_back(bottomRightFront);
	normals.push_back(bottomRightFront);
	//bottom left front x,y,z vertex
	vertices.push_back(bottomLeftFront);
	normals.push_back(bottomLeftFront);
	//face 2
	//bottom left back x,y,z vertex
	vertices.push_back(bottomLeftBack);
	normals.push_back(bottomLeftBack);
	//bottom right back x,y,z vertex
	vertices.push_back(bottomRightBack);
	normals.push_back(bottomRightBack);
	//bottom left front x,y,z vertex
	vertices.push_back(bottomLeftFront);
	normals.push_back(bottomLeftFront);

	//front
	//face 1
	//top right front x,y,z vertex
	vertices.push_back(topRightFront);
	normals.push_back(topRightFront);
	//top left front x,y,z vertex
	vertices.push_back(topLeftFront);
	normals.push_back(topLeftFront);
	//bottom right front x,y,z vertex
	vertices.push_back(bottomRightFront);
	normals.push_back(bottomRightFront);
	//face 2
	//top left front x,y,z vertex
	vertices.push_back(topLeftFront);
	normals.push_back(topLeftFront);
	//bottom left front x,y,z vertex
	vertices.push_back(bottomLeftFront);
	normals.push_back(bottomLeftFront);
	//bottom right front x,y,z vertex
	vertices.push_back(bottomRightFront);
	normals.push_back(bottomRightFront);

	//back
	//face 1
	//top right back x,y,z vertex
	vertices.push_back(topRightBack);
	normals.push_back(topRightBack);
	//top left back x,y,z vertex
	vertices.push_back(topLeftBack);
	normals.push_back(topLeftBack);
	//bottom right back x,y,z vertex
	vertices.push_back(bottomRightBack);
	normals.push_back(bottomRightBack);
	//face 2
	//top left back x,y,z vertex
	vertices.push_back(topLeftBack);
	normals.push_back(topLeftBack);
	//top right back x,y,z vertex
	vertices.push_back(bottomRightBack);
	normals.push_back(bottomRightBack);
	//bottom left back x,y,z vertex
	vertices.push_back(bottomLeftBack);
	normals.push_back(bottomLeftBack);

	//top
	//face 1
	//bottom right back x,y,z vertex
	vertices.push_back(topRightBack);
	normals.push_back(topRightBack);
	//bottom right front x,y,z vertex
	vertices.push_back(topRightFront);
	normals.push_back(topRightFront);
	//bottom left front x,y,z vertex
	vertices.push_back(topLeftFront);
	normals.push_back(topLeftFront);
	//face 2
	//top left back x,y,z vertex
	vertices.push_back(topLeftBack);
	normals.push_back(topLeftBack);
	//top right back x,y,z vertex
	vertices.push_back(topRightBack);
	normals.push_back(topRightBack);
	//top left front x,y,z vertex
	vertices.push_back(topLeftFront);
	normals.push_back(topLeftFront);

	//left
	//face 1
	//bottom left back x,y,z vertex
	vertices.push_back(topLeftBack);
	normals.push_back(topLeftBack);
	//bottom left front x,y,z vertex
	vertices.push_back(bottomLeftFront);
	normals.push_back(bottomLeftFront);
	//bottom left front x,y,z vertex
	vertices.push_back(bottomLeftBack);
	normals.push_back(bottomLeftBack);
	//face 2
	//bottom left back x,y,z vertex
	vertices.push_back(topLeftFront);
	normals.push_back(topLeftFront);
	//bottom right back x,y,z vertex
	vertices.push_back(topLeftBack);
	normals.push_back(topLeftBack);
	//bottom left front x,y,z vertex
	vertices.push_back(bottomLeftFront);
	normals.push_back(bottomLeftFront);

	//right
	//face 1
	//bottom left back x,y,z vertex
	vertices.push_back(topRightBack);
	normals.push_back(topRightBack);
	//bottom Right front x,y,z vertex
	vertices.push_back(bottomRightFront);
	normals.push_back(bottomRightFront);
	//bottom Right front x,y,z vertex
	vertices.push_back(bottomRightBack);
	normals.push_back(bottomRightBack);
	//face 2
	//bottom Right back x,y,z vertex
	vertices.push_back(topRightFront);
	normals.push_back(topRightFront);
	//bottom right back x,y,z vertex
	vertices.push_back(topRightBack);
	normals.push_back(topRightBack);
	//bottom Right front x,y,z vertex
	vertices.push_back(bottomRightFront);
	normals.push_back(bottomRightFront);
}

GEOMETRY::RayIntersectionInfo GEOMETRY::Box::rayIntersectionInfo(const Ray& ray) const
{
	return RayIntersectionInfo();
}
