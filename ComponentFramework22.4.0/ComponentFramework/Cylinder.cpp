#include "Cylinder.h"
#include "Matrix.h"
#include "MMath.h"

using namespace MATH;
using namespace GEOMETRY;

float Cylinder::calculateHeight() { //using Pythagoras Theorem to find the height of the cylinder
	//abs(capCentrePosB.z - capCentrePosA.z);
	float xSquared = abs(capCentrePosA.x - capCentrePosB.x) * (abs(capCentrePosA.x - capCentrePosB.x));
	float ySquared = abs(capCentrePosA.y - capCentrePosB.y) * (abs(capCentrePosA.y - capCentrePosB.y));
	float zSquared = abs(capCentrePosA.z - capCentrePosB.z) * (abs(capCentrePosA.z - capCentrePosB.z));
	return (sqrt(xSquared + ySquared + zSquared));
}

MATH::Vec3 Cylinder::getNormalizedVector() { //returns the normalized vector that represents the axis
	MATH::Vec3 normalizedVector;
	normalizedVector.x = abs(capCentrePosA.x - capCentrePosB.x);
	normalizedVector.y = abs(capCentrePosA.y - capCentrePosB.y);
	normalizedVector.z = abs(capCentrePosA.z - capCentrePosB.z);
	normalizedVector = VMath::normalize(normalizedVector);
	return normalizedVector;
}

void Cylinder::generateVerticesAndNormals()
{
	const float height = calculateHeight(); //get the height
	//every variable with delta determines how often vertices are placed to make the "mesh" of the primitive
	const float deltaTheta = 360.0f / 180.0f; //(360.0f / 180.0f) places a vertex every 2 degrees of the circle
	const float deltaHeight = height / 10; //(height / 10) places ten sets of circles to represent the cylinder in the z axis
	const float deltaRadius = r / 10; //(r / 10) places ten sets of circles to represent the cylinder top and bottom caps

	Vec3 circle(0.0f, 0.0f, 0.0f); //make a vector to represent the circles in the visuals

	//put a vertex in the center of the top and the bottom of the cylinder
	vertices.push_back(capCentrePosA);
	normals.push_back(capCentrePosA);
	vertices.push_back(capCentrePosB);
	normals.push_back(capCentrePosB);

	Vec3 normal = getNormalizedVector(); //get the normalized vector
	//cylinder top & bottom
	for (int cylinderCap = 0; cylinderCap != 2; cylinderCap++) { //this runs the loop twice
		for (float currentcircle = deltaRadius; currentcircle <= r; currentcircle += deltaRadius) { //this makes the circle expand its radius 10 times to represent the top cap
			circle = Vec3(0.0f, currentcircle, 0.0f); //set the circle with the new radius
			for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta) { //this can changed to any loop that loops 180 times and update thetaDeg
				MATH::Matrix3 rotationMatrix;
				rotationMatrix = MATH::MMath::rotate(deltaTheta, normal); //rotate the vertex by deltaTheta along the normal of the cylinder
				circle = rotationMatrix * circle; //update the circle with the rotation
				if (cylinderCap == 0) {
					vertices.push_back(circle + capCentrePosA); //if drawing the top cap - add the vertex
				}
				else {
					vertices.push_back(circle + capCentrePosB); //if drawing the bottom cap - add the vertex
				}
				normals.push_back(circle);
			}
		}
	}

	//cylinder wall 
	for (float currentLayer = 0.0f; currentLayer <= height; currentLayer += deltaHeight) { //run 10 times and move the circles z by the deltaHeight
		for (float thetaDeg = 0.0f; thetaDeg <= 360.0f; thetaDeg += deltaTheta) { //this can changed to any loop that loops 180 times
			MATH::Matrix3 rotationMatrix;
			rotationMatrix = MATH::MMath::rotate(deltaTheta, normal); //rotate the vertex by deltaTheta along the normal of the cylinder
			circle = rotationMatrix * circle; //update the circle with the rotation
			vertices.push_back(circle + capCentrePosA); //move the vertex point based on the position of the shape
			normals.push_back(circle);
		}
		circle = normal * -currentLayer; //update the layer in the circle in the direction of the normal
		circle.y = r; //set the circle to have the radius as the y value
	}
}