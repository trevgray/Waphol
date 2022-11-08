#pragma once
#include "Component.h"

#include "Vector.h"
#include <vector>

#include "Graph.h"
//#include <iostream>

class NavigationMesh {
public:
	NavigationMesh();
	~NavigationMesh();

	void Initialize(MATH::Vec3 topLeftCorner_, MATH::Vec3 bottomRightCorner_, std::vector<std::string> ignoreActors_);
	void OnCreate();

	void DebugDraw();

	Graph GetVoronoiGraph() { return voronoiGraph; }
private:
	Graph voronoiGraph;
	MATH::Vec3 bottomLeftCorner; 
	MATH::Vec3 topRightCorner; 
	std::vector<std::string> ignoreActors;
};