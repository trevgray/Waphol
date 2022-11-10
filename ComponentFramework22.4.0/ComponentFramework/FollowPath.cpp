#include "FollowPath.h"
#include "EngineManager.h"
#include "PhysicsBodyComponent.h"

FollowPath::FollowPath(float targetRadius_, float slowRadius_, float timeToTarget_) : Arrive("", targetRadius_, slowRadius_, timeToTarget_) {
	currentNode = 0;
}

FollowPath::~FollowPath() {
}

bool FollowPath::OnCreate() {
	target = std::make_shared<TransformComponent>(nullptr);
	navMesh = EngineManager::Instance()->GetActorManager()->GetNavigationMesh();
	//find parent actor
	/*for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second != nullptr && actor.first == targetName) {
			target = actor.second->GetComponent<TransformComponent>();
			break;
		}
	}*/
	return true;
}

void FollowPath::SetGoal(int goalNodeLabel) {
	if (navMesh == nullptr) {
		std::cout << "Error: Set the NavMesh before setting a goal" << std::endl;
		return;
	}
	path.clear();

	int current = currentNode;
	std::vector<int> cameFrom = navMesh->GetVoronoiGraph().AStar(goalNodeLabel, currentNode);
	while (current != goalNodeLabel) {
		//std::cout << current << " " << goalNodeLabel << std::endl;
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
		currentNode = path.back();
		path.pop_back();
	}

	target->SetPosition(targetPosition);

	return Arrive::GetSteering(actor_);
	}
