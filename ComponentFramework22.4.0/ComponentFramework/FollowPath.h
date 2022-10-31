#pragma once
#include "Arrive.h"
#include "NavigationMesh.h"

class FollowPath : public Arrive {
public:
	FollowPath(float targetRadius_, float slowRadius_, float timeToTarget_);
	virtual ~FollowPath();
	bool OnCreate();
	void SetGoal(int nodeLabel);
	virtual SteeringOutput GetSteering(Ref<Actor> actor_);

	inline void SetNavMesh(Ref<NavigationMesh> navMesh_) { navMesh = navMesh_; }

private:
	std::vector<int> path;
	Ref<NavigationMesh> navMesh;

	int currentNode;
};

