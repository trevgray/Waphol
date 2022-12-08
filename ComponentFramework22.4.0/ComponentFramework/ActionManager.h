#pragma once
#include "Action.h"

class ActionManager {
public:
	ActionManager();
	~ActionManager();

	void ScheduleAction(Action* action);
	void Execute(float deltaTime);
private:
	std::vector<Action*> actionQueue;
	float timer;
};

