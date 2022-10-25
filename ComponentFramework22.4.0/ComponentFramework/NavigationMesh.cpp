#include "NavigationMesh.h"
#include <iostream>

NavigationMesh::NavigationMesh() {
	memset(&diagram, 0, sizeof(jcv_diagram));
}

NavigationMesh::~NavigationMesh() {
	jcv_diagram_free(&diagram);
}

void NavigationMesh::Initialize() {

}