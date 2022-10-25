#pragma once

#include "MMath.h"

class Node {
private:
	int label;
	MATH::Vec3 position;
public:
	Node() { label = -1; }
	Node(int label_, MATH::Vec3 position_ = MATH::Vec3()) { 
		label = label_; 
		position = position_;
	}
	~Node() {}
	int GetLabel() { return label; }
	MATH::Vec3 GetPos() { return position; }
};