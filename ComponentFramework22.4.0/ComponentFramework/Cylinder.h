#pragma once
#include "Shape.h"
namespace GEOMETRY {
	struct Cylinder : public Shape {
		float r = 1.0f;
		MATH::Vec3 capCentrePosA;
		MATH::Vec3 capCentrePosB;
		/// Just a little utility to populate a Sphere. The position of the centre of the sphere is the Vec3 inherited
		inline void set(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_) {
			capCentrePosA = capCentrePosA_; //use the overload
			capCentrePosB = capCentrePosB_;
			r = r_;
			generateVerticesAndNormals(); //generate visual for the primitive
			StoreMeshData(GL_POINTS);
		}

		//Default constructor sets the center position of sphere endA to something other than zero, generates the vertices and normal for the shape, and stores the mesh data in order to render
		Cylinder() {
			set(1.0f, MATH::Vec3(0.0f, 0.0f, 1.0f), MATH::Vec3());
		}

		Cylinder(float r_, MATH::Vec3 capCentrePosA_, MATH::Vec3 capCentrePosB_) {
			set(r_, capCentrePosA_, capCentrePosB_);
		}

		Cylinder(const Cylinder& c) { //create copy of cylinder object
			set(c.r, c.capCentrePosA, c.capCentrePosB);
		}

		~Cylinder() {}

		float calculateHeight();

		MATH::Vec3 getNormalizedVector();

		void generateVerticesAndNormals() override;

		// REFERENCE: Real Time Collision Detection by Ericson for all the ray-cylinder stuff. Resist the urge to normalize the ray direction!
		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;

		RayIntersectionInfo CheckInfiniteCylinder(const Ray& ray) const; 

		RayIntersectionInfo CheckEndCap(const Ray& ray, float t) const;
	};
}

