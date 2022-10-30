#include "FollowPath.h"
#include "EngineManager.h"
#include "PhysicsBodyComponent.h"

FollowPath::FollowPath(std::string targetName_, float targetRadius_, float slowRadius_, float timeToTarget_) : Arrive(targetName_, targetRadius_, slowRadius_, timeToTarget_) {

}

FollowPath::~FollowPath() {
}

bool FollowPath::OnCreate() {
	//find parent actor
	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second != nullptr && actor.first == targetName) {
			target = actor.second->GetComponent<TransformComponent>();
			break;
		}
	}
	return true;
}

void FollowPath::SetGoal(int goalNodeLabel) {
	int current = 0;

	std::vector<int> cameFrom = navMesh->GetVoronoiGraph().Dijkstra(goalNodeLabel, 0);
	while (current != goalNodeLabel) {
		path.push_back(current);
		current = cameFrom[current];
	}
	path.push_back(goalNodeLabel);
	std::reverse(path.begin(), path.end());
}

SteeringOutput FollowPath::GetSteering(Ref<Actor> actor_) {
	Vec3 targetPosition;

	if (path.size() == 0) { 
		//actor_->GetComponent<PhysicsBodyComponent>()->SetVel(Vec3());
		return SteeringOutput(); 
	}

	//target_position = path->getCurrentNodePosition();
	targetPosition = navMesh->GetVoronoiGraph().GetNode(path.back()).GetPos();

	if (VMath::distance(actor_->GetComponent<TransformComponent>()->GetPosition(), targetPosition) <= targetRadius) {
		// incremented for next steering request
		//path->incrementCurrentNode(1);
		path.pop_back();
	}

	target->SetPosition(targetPosition);

	return Arrive::GetSteering(actor_);
	}
