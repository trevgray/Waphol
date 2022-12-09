#pragma once
#include "Action.h"

class ActionManager {
public:
	ActionManager();
	~ActionManager();

	void ScheduleAction(Action* action);
	void Execute(float deltaTime);

	void SetOwner(Ref<Actor> owner_) { owner = owner_; }
	std::string GetActiveAction() { return actionQueue.front()->GetValue(); }
private:
	Ref<Actor> owner;
	void ExecuteAction(Action* action);
	std::vector<Action*> actionQueue;
	float timer;
};

