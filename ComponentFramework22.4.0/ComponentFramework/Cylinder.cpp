#include "Cylinder.h"
#include "Matrix.h"
#include "MMath.h"

#include "QuadraticSolution.h"

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
	normalizedVector.x = capCentrePosA.x - capCentrePosB.x;
	normalizedVector.y = capCentrePosA.y - capCentrePosB.y;
	normalizedVector.z = capCentrePosA.z - capCentrePosB.z;
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

RayIntersectionInfo GEOMETRY::Cylinder::rayIntersectionInfo(const Ray& ray) const {
	RayIntersectionInfo rayInfo;
	rayInfo = CheckInfiniteCylinder(ray);

	RayIntersectionInfo rayEndCapCheck; //this is done so I don't overwrite the t or interaction point

	if (rayInfo.isIntersected == true) {
		rayEndCapCheck = CheckEndCap(ray, rayInfo.t);
	}

	if (rayEndCapCheck.isIntersected == true) {
		rayInfo.isIntersected = true;
	}
	else {
		rayInfo.isIntersected = false;
	}

	return rayInfo;
}

RayIntersectionInfo GEOMETRY::Cylinder::CheckInfiniteCylinder(const Ray& ray) const {
	RayIntersectionInfo rayInfo;
	//Solve the quadratic function for this interaction
	//Used Ray Cylinder Intersections (Game Physics 3) Part A

	//Set commonly used vectors in the equations
	Vec3 AB = capCentrePosB - capCentrePosA;
	AB = VMath::normalize(AB);
	Vec3 AS = ray.start - capCentrePosA;
	//Calculating A B C coefficients for quadratic equation to solve for t (algebra explained in Ray Cylinder Intersections Part A)
	const float a = VMath::dot(ray.dir, ray.dir) - (VMath::dot(ray.dir, AB) * VMath::dot(ray.dir, AB)); //powf(VMath::dot(ray.dir, AB), 2); also works
	const float b = 2.0f * (VMath::dot(AS, ray.dir) - (VMath::dot(ray.dir, AB) * VMath::dot(AS, AB)));
	const float c = VMath::dot(AS, AS) - (VMath::dot(AS, AB) * VMath::dot(AS, AB)) - r * r; //powf(VMath::dot(AS, AB), 2);

	QuadraticSolution soln = soln.SolveQuadratic(a, b, c);

	//Gets the closer interaction point (Explained in Game Physics 3 Ray Sphere Collisions Slide 9)
	//The smaller t value is where the ray inters the cylinder, and the larger t value is where the ray exits the cylinder.
	if (soln.numSolutions == NumSolutions::zeroRoots) {
		rayInfo.isIntersected = false;
	}
	else if (soln.secondRoot < 0.0f) {
		rayInfo.isIntersected = true;
		//ray is going backwards
		//second root is the smallest absolute value
		rayInfo.intersectionPoint = ray.currentPosition(soln.secondRoot);
		rayInfo.t = soln.secondRoot;
	}
	else {
		rayInfo.isIntersected = true;
		rayInfo.intersectionPoint = ray.currentPosition(soln.firstRoot);
		rayInfo.t = soln.firstRoot;
	}

	return rayInfo;
}

RayIntersectionInfo GEOMETRY::Cylinder::CheckEndCap(const Ray& ray, float t) const {
	RayIntersectionInfo rayInfo;
	rayInfo.isIntersected = false; //set it to false, only update it if the interaction is valid

	//Used Ray Cylinder Intersections (Game Physics 3) Part B

	//Set commonly used vectors in the equations
	Vec3 AB = capCentrePosB  - capCentrePosA;
	Vec3 AS = ray.start - capCentrePosA;
	Vec3 AP = ray.currentPosition(t) - capCentrePosA;

	//Check the angle between AP and AB
	//when vector AP, AB are 90 degrees for each other - the dot product = 0 
	//dot(AP, AB) is < 0 when the angle is greater than 90
	//dot(AP, AB) is > 0 when the angle is less than 90
	float aAngleCheck = VMath::dot(AP, AB);

	//Check if the vector is pointing towards the end cap
	//dot(ray.dir, AB) will be greater than 0 if ray.dir is towards the end cap
	//because the angle between ray.dir and AB would be acute (<90)
	float aDirectionCheck = VMath::dot(ray.dir, AB);

	//Check if the point on the circle's plane is within the circle's radius
	//Check Ray Cylinder Intersections 2 of 2 page 3 (after t equation)
	float aDistanceCheck = VMath::mag(AS - (VMath::dot(AS, AB) / VMath::dot(ray.dir, AB)) * ray.dir);
	aDistanceCheck = VMath::mag(AS + t * ray.dir);
	//float aDistanceCheck = VMath::mag(AS + t * ray.dir); also works because t = -(VMath::dot(AS, AB) / VMath::dot(ray.dir, AB))

	//Do the same thing with the B cap now
	Vec3 BA = capCentrePosA - capCentrePosB;
	Vec3 BS = ray.start - capCentrePosB;
	Vec3 BP = ray.currentPosition(t) - capCentrePosB;
	float bAngleCheck = VMath::dot(BP, BA);
	float bDirectionCheck = VMath::dot(ray.dir, BA);
	float bDistanceCheck = VMath::mag(BS - (VMath::dot(BS, BA) / VMath::dot(ray.dir, BA)) * ray.dir);

	//if both the angle checks are acute (dot product greater than zero) then the intersection is between the end points of the cylinder (on the side of the cylinder)
	if (aAngleCheck > 0.0f && bAngleCheck > 0.0f) {
		rayInfo.isIntersected = true;
	}
	//Check if the vector hits the end cap with all the defined variables
	//Check Ray Cylinder Intersections 2 of 2 page 5
	else if ((aAngleCheck < 0.0f) && (aDirectionCheck > 0.0f) && (aDistanceCheck <= r) || /*A Cap Check*/
		(bAngleCheck < 0.0f) && (bDirectionCheck > 0.0f) && (bDistanceCheck <= r)) /*B Cap Check*/ {
		rayInfo.isIntersected = true;
	}

	return rayInfo; //done :)
}
