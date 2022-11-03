#pragma once
#include "Shape.h"
namespace GEOMETRY {
	struct Capsule : public Shape {
		float r = 1.0f;
		MATH::Vec3 sphereCentrePosA;
		MATH::Vec3 sphereCentrePosB;
		/// Just a little utility to populate a Sphere. The position of the centre of the sphere is the Vec3 inherited
		inline void set(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_) {
			sphereCentrePosA = sphereCentrePosA_; //use the overload
			sphereCentrePosB = sphereCentrePosB_;
			r = r_;
			generateVerticesAndNormals(); //generate visual for the primitive
			StoreMeshData(GL_POINTS);
		}

		//Default constructor sets the centre position of cap A to something other than zero, generates the vertices and normal for the shape, and stores the mesh data in order to render
		Capsule() {
			set(1.0f, MATH::Vec3(0.0f, 0.0f, 1.0f), MATH::Vec3());
		}

		Capsule(float r_, MATH::Vec3 sphereCentrePosA_, MATH::Vec3 sphereCentrePosB_) {
			set(r_, sphereCentrePosA_, sphereCentrePosB_);
		}

		Capsule(const Capsule& c) { //create copy of cylinder object
			set(c.r, c.sphereCentrePosA, c.sphereCentrePosB);
		}

		~Capsule() {}

		float calculateHeight();
		MATH::Vec3 getNormalizedVector();
		// Fill the vertices and normals list with Vec3's to represent a capsule
		void generateVerticesAndNormals() override;

		RayIntersectionInfo rayIntersectionInfo(const Ray& ray) const override;

		RayIntersectionInfo checkEndSphere(MATH::Vec3 sphereCentre, const Ray& ray) const; 
		RayIntersectionInfo checkHalfSphere(const RayIntersectionInfo& rayInfoFullSphere, const MATH::Vec3& sphereCentre, const MATH::Vec3& sphereOtherCentre) const;
	};
}