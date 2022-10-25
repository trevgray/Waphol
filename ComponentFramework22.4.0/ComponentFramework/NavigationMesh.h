#pragma once
#define JC_VORONOI_IMPLEMENTATION
#include "jc_voronoi.h"
#include "jc_voronoi_clip.h"

class NavigationMesh {
public:
	NavigationMesh();
	~NavigationMesh();

	void Initialize();
private:
	jcv_diagram diagram;
};