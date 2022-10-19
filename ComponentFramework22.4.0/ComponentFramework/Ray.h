#pragma once
#include "Vector.h"
#include "VMath.h"

namespace GEOMETRY {
	struct RayIntersectionInfo {
		bool isIntersected = false;
		MATH::Vec3 intersectionPoint;
		float t = 0.0f; //handy to know if we are going forward or backwards

		void print() {
			if (isIntersected) {
				std::cout << "Ray intersection with t value " << t << " at position ";
				intersectionPoint.print();
			}
			else {
				std::cout << "No ray intersection" << std::endl;
			}
		}
	};

	struct Ray {
		MATH::Vec3 dir;
		MATH::Vec3 start;
		Ray() {
			dir.set(0.0f, 0.0f, -1.0f);
			start.set(0.0f, 0.0f, 0.0f);
		}
		Ray(MATH::Vec3& start_, MATH::Vec3& dir_) {
			set(start_, dir_);
		}

		void set(const MATH::Vec3& start_, const MATH::Vec3& dir_) {
			start = start_;
			dir = dir_;
		}

		MATH::Vec3 currentPosition(float t) const { return start + t * dir; }
	};
}