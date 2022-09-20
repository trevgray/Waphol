#include "Capsule.h"
#include "Matrix.h"
#include "MMath.h"

using namespace MATH;
using namespace GEOMETRY;

float GEOMETRY::Capsule::calculateHeight() { //using Pythagoras Theorem to find the height of the capsule
	//abs(capCentrePosB.z - capCentrePosA.z);
	float xSquared = abs(sphereCentrePosA.x - sphereCentrePosB.x) * (abs(sphereCentrePosA.x - sphereCentrePosB.x));
	float ySquared = abs(sphereCentrePosA.y - sphereCentrePosB.y) * (abs(sphereCentrePosA.y - sphereCentrePosB.y));
	float zSquared = abs(sphereCentrePosA.z - sphereCentrePosB.z) * (abs(sphereCentrePosA.z - sphereCentrePosB.z));
	return (sqrt(xSquared + ySquared + zSquared) /*+ (r * 2)*/);
}

MATH::Vec3 GEOMETRY::Capsule::getNormalizedVector() { //returns the normalized vector that represents the axis
	MATH::Vec3 normalizedVector;
	normalizedVector.x = sphereCentrePosA.x - sphereCentrePosB.x;
	normalizedVector.y = sphereCentrePosA.y - sphereCentrePosB.y;
	normalizedVector.z = sphereCentrePosA.z - sphereCentrePosB.z;
	normalizedVector = VMath::normalize(normalizedVector);
	return normalizedVector;
}

void GEOMETRY::Capsule::generateVerticesAndNormals() {
	const float height = calculateHeight(); //get the height
	//every variable with delta determines how often vertices are placed to make the "mesh" of the primitive
	const float deltaTheta = 1.0f;
	const float deltaPhi = 360.0f / 30.0f; //(360.0f / 30.0f) places a vertex every 12 degrees of the circle
	const float deltaHeight = height / 10; //(height / 10) places ten sets of circles to represent the cylinder in the z axis

	Vec3 circle(0.0f, 0.0f, 0.0f); //make a vector to represent the circles in the visuals

	//put a vertex in the center of the top and the bottom of the cylinder
	vertices.push_back(sphereCentrePosA);
	normals.push_back(sphereCentrePosA);
	vertices.push_back(sphereCentrePosB);
	normals.push_back(sphereCentrePosB);

	Vec3 normal = getNormalizedVector(); //get the normalized vector
	//Capsule top & bottom
	for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta) {
		Vec3 circle(r * sin(thetaDeg * DEGREES_TO_RADIANS), r * cos(thetaDeg * DEGREES_TO_RADIANS), 0.0f); //update the circle every frame with the next vertex location
		for (float phiDeg = 0.0f; phiDeg <= 360.0f; phiDeg += deltaPhi) {
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(deltaPhi, normal); //rotate the vertex by deltaTheta along the normal of the cylinder
			circle = rotationMatrix * circle;//update the circle with the rotation

			/*int topDegree = 90 + ((0.9 * abs(normal.y - 1)) * 100); 
			int bottomDegree = 180 + ((0.9 * abs(normal.y - 1)) * 100);*/
			if (thetaDeg < 90) { //top circle
				vertices.push_back(circle + Vec3(sphereCentrePosA.x, sphereCentrePosA.y, sphereCentrePosA.z)); //move the vertex point based on the position of the top circle
			}
			else if (thetaDeg < 180) { //bottom circle
				vertices.push_back(circle + Vec3(sphereCentrePosB.x, sphereCentrePosB.y, sphereCentrePosB.z)); //move the vertex point based on the position of the bottom circle
			}
			normals.push_back(circle);
		}
	}


	//Capsule wall 
	for (float currentLayer = 0.0f; currentLayer <= height; currentLayer += deltaHeight) { //run 10 times and move the circles z by the deltaHeight
		for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta) { //this can changed to any loop that loops 180 times and update thetaDeg
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(deltaTheta, normal); //rotate the vertex by deltaTheta along the normal of the cylinder
			circle = rotationMatrix * circle; //update the circle with the rotation
			vertices.push_back(circle + (sphereCentrePosA)); //move the vertex point based on the position of the shape
			normals.push_back(circle);
		}
		circle = normal * -currentLayer; //update the layer in the circle in the direction of the normal
		circle.z = r; //set the circle to have the radius as the z value
	}
}
