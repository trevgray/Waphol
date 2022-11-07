#include "Box.h"
#include "QMath.h"
#include <array>

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

GEOMETRY::RayIntersectionInfo GEOMETRY::Box::rayIntersectionInfo(const Ray& ray) const {
	RayIntersectionInfo rayInfo;

	std::array<GEOMETRY::Slab, 3> slabs = {
		GEOMETRY::Slab{MATH::Vec3(1.0f, 0.0f, 0.0f), -halfExtent.x, halfExtent.x},
		GEOMETRY::Slab{MATH::Vec3(0.0f, 1.0f, 0.0f), -halfExtent.y, halfExtent.y},
		GEOMETRY::Slab{MATH::Vec3(0.0f, 0.0f, 1.0f), -halfExtent.z, halfExtent.z} 
	};

	float tx, ty, tz;
	//X Intersection
	if (ray.dir.x == 0) { //Slide 9
		tx = NULL;
	}
	else if (ray.dir.x > 0) {
		tx = (slabs[0].distNear - ray.start.x) / ray.dir.x;
	}
	else if (ray.dir.x < 0) {
		tx = (slabs[0].distFar - ray.start.x) / ray.dir.x;
	}
	//tx check
	if (tx < 0 || tx == 0) { //Slide 10 | This is checking t to see if the ray is facing the plane or inside the place
		tx = NULL;
	}
	//Y Intersection
	if (ray.dir.y == 0) {
		ty = NULL;
	}
	else if (ray.dir.y > 0) {
		ty = (slabs[1].distNear - ray.start.y) / ray.dir.y;
	}
	else if (ray.dir.y < 0) {
		ty = (slabs[1].distFar - ray.start.y) / ray.dir.y;
	}
	//ty check
	if (ty < 0 || ty == 0) {
		ty = NULL;
	}
	//Z Intersection
	if (ray.dir.z == 0) {
		tz = NULL;
	}
	else if (ray.dir.z > 0) {
		tz = (slabs[2].distNear - ray.start.z) / ray.dir.z;
	}
	else if (ray.dir.z < 0) {
		tz = (slabs[2].distFar - ray.start.z) / ray.dir.z;
	}
	//tz check
	if (tz < 0 || tz == 0) {
		tz = NULL;
	}
	MATH::Vec3 p = MATH::Vec3();
	//Ray Checks
	if (tx != NULL) {
		p = ray.currentPosition(tx);
		if (isInside(p) == true) {
			rayInfo.isIntersected = true;
			rayInfo.t = tx;
			rayInfo.intersectionPoint = p;
			return rayInfo;
		}
	}
	else if (ty != NULL) {
		p = ray.currentPosition(ty);
		if (isInside(p) == true) {
			rayInfo.isIntersected = true;
			rayInfo.t = ty;
			rayInfo.intersectionPoint = p;
			return rayInfo;
		}
	}
	else if (tz != NULL) {
		p = ray.currentPosition(tz);
		if (isInside(p) == true) {
			rayInfo.isIntersected = true;
			rayInfo.t = tz;
			rayInfo.intersectionPoint = p;
			return rayInfo;
		}
	}

	return rayInfo;
}

bool GEOMETRY::Box::isInside(MATH::Vec3 p) const { //Slide 11 | If either of these conditions fails, then no intersection takes place within the face.
	if (-halfExtent.x <= p.x && p.x <= halfExtent.x && 
		-halfExtent.y <= p.y && p.y <= halfExtent.y &&
		-halfExtent.z <= p.z && p.x <= halfExtent.z) {
		return true;
	}
	else {
		return false;
	}
}
