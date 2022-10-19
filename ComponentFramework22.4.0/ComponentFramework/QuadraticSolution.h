#pragma once
#include <iostream>
#include <algorithm>

namespace GEOMETRY {
	enum class NumSolutions { zeroRoots = 0, oneRoot, twoRoots };
	struct QuadraticSolution {

		NumSolutions numSolutions { NumSolutions::zeroRoots };
		// I'll use first root as the smallest of the two. Set them both to zero to begin with
		float firstRoot = 0.0f;
		float secondRoot = 0.0f;
		void print() { // handy method that prints out the number of solutions and their value(s)
			if (numSolutions == NumSolutions::zeroRoots) {
				std::cout << "No roots were found" << std::endl;
			}
			else if (numSolutions == NumSolutions::oneRoot) {
				std::cout << "One root was found: " << firstRoot << std::endl;
			}
			else if (numSolutions == NumSolutions::twoRoots) {
				std::cout << "Two roots were found - Root One: " << firstRoot << " Root Two: " << secondRoot << std::endl;
			}
		}

		QuadraticSolution SolveQuadratic(float a, float b, float c) { //To actually solve the quadratic equation, we don’t need an entire class - just a function
			QuadraticSolution solution;
			float discriminant = b * b - 4.0f * a * c;

			if (discriminant < 0.0f) {
				return solution; //zero roots
			}
			else if (fabs(discriminant) < FLT_MIN) { //it grazed the sphere or started inside the sphere
				solution.numSolutions = NumSolutions::oneRoot;
				solution.firstRoot = (-b + sqrt(discriminant)) / (2.0f * a); //quadratic function for +
				return solution;
			}
			else {
				solution.numSolutions = NumSolutions::twoRoots;
				float x1 = (-b + sqrt(discriminant)) / (2.0f * a); //quadratic function for + soln
				float x2 = (-b - sqrt(discriminant)) / (2.0f * a); //quadratic function for - soln
				//Order the roots so the first root is the smallest
				solution.firstRoot = std::min(x1, x2);
				solution.secondRoot = std::max(x1, x2);
				return solution;
			}
		}
	};
}