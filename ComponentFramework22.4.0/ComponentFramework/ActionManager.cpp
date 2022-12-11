#include "ActionManager.h"

#include "SteeringComponent.h"
#include "PhysicsBodyComponent.h"

ActionManager::ActionManager() {
	timer = 0.0f;
	owner = nullptr;
}

ActionManager::~ActionManager() {}

void ActionManager::ScheduleAction(Action* action) {
	bool actionInQueue = false;
	for (Action* actionIterator : actionQueue) {
		if (actionIterator->GetValue() == action->GetValue()) {
			actionInQueue = true;
		}
	}
	if (actionInQueue == false) {
		actionQueue.push_back(action);
	}
}

void ActionManager::Execute(float deltaTime) {
	Action* active = actionQueue.front();
	timer += deltaTime;

	//Remove expired actions from queue
	int actionIterator = 0;
	for (Action* action : actionQueue) {
		if (action->IsExpired(timer) == true) {
			actionQueue.erase(actionQueue.begin() + actionIterator);
			break;
		}
		actionIterator++;
	}
	//Check if the action is complete - if yes, move to the next action
	if (active->IsComplete(timer) == true) {
		if (actionQueue.size() > 0) {
			actionQueue.erase(actionQueue.begin());
			//Push the highest priority to the front of the vector
			int actionIterator = 0;
			for (Action* action : actionQueue) {
				if (action->priority > actionQueue.front()->priority) {
					actionQueue.insert(actionQueue.begin(), action); //insert at the front
					actionQueue.erase(actionQueue.begin() + actionIterator + 1); //erase it from the slot it was in
					break;
				}
				actionIterator++;
			}
		}
		//reset the timer
		timer = 0.0f;
	}
	//Start the action if it has not started
	if (active->isExecuted == false) {

		ExecuteAction(active);
	}
}

void ActionManager::ExecuteAction(Action* action) {
	action->isExecuted = true;
	std::cout << action->GetValue() << std::endl;
	if (action->GetValue() == "SEEK") {
		owner->GetComponent<SteeringComponent>()->SetIsActive(true);
	}
	else if (action->GetValue() == "DO_NOTHING") {
		owner->GetComponent<SteeringComponent>()->SetIsActive(false);
		owner->GetComponent<PhysicsBodyComponent>()->SetVel(Vec3());
		owner->GetComponent<PhysicsBodyComponent>()->SetAccel(Vec3());
	}
	else if (action->GetValue() == "ATTACK") {
		owner->GetComponent<SteeringComponent>()->SetIsActive(false);
		owner->GetComponent<PhysicsBodyComponent>()->SetVel(Vec3());
		owner->GetComponent<PhysicsBodyComponent>()->SetAccel(Vec3());

	}
}
