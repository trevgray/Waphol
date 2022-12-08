#include "ActionManager.h"

ActionManager::ActionManager() {
	timer = 0.0f;
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
		}
		//reset the timer
		timer = 0.0f;
	}
	//Start the action if it has not started
	if (active->isExecuted == false) {
		active->Execute();
	}
}
