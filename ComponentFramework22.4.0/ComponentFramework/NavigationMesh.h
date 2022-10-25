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

	void Initialize(MATH::Vec3 topLeftCorner, MATH::Vec3 bottomRightCorner, std::vector<std::string> ignoreActors);

	Graph GetVoronoiGraph() { return voronoiGraph; }
private:
	Graph voronoiGraph;
};