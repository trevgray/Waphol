#pragma once
#include "Shape.h"
#include "Quaternion.h"
namespace GEOMETRY {
	struct Box : public Shape {
		// Represent an Orientated Bounding Box by center position, half extents, and orientation. REFERENCE: Real Time Collision Detection by Ericson
		// Ericson recommends keeping the orientation as a matrix for fast collision detection,but I love quaternions too much
		MATH::Vec3 center; 
		MATH::Vec3 halfExtent; 
		MATH::Quaternion orientation;
		// Default constructor sets the half extents along x, y, and z to something other than zero, generates the vertices and normal for the shape, and stores the mesh data in order to render
		Box() {
			set(MATH::Vec3(), MATH::Vec3(1.0f,1.0f,1.0f), MATH::Quaternion());
		}
		Box(MATH::Vec3 centre_, MATH::Vec3 halfExtents_, MATH::Quaternion orientation_) {
			set(centre_, halfExtents_, orientation_);
		}

		void set(MATH::Vec3 center_, MATH::Vec3 halfExtent_, MATH::Quaternion orientation_) {
			center = center_; //use the overload
			halfExtent = halfExtent_;
			orientation = orientation_;
			generateVerticesAndNormals(); //generate visual for the primitive
			StoreMeshData(GL_TRIANGLES);
		}

		// Fill the vertices and normals list with Vec3's to represent a box
		void generateVerticesAndNormals() override;

		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;
	};
}