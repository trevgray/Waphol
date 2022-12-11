#pragma once
#include "Action.h"

class ActionManager {
public:
	ActionManager();
	~ActionManager();

	void ScheduleAction(Action* action);
	void Execute(float deltaTime);

	void SetOwner(Ref<Actor> owner_) { owner = owner_; }
	std::string GetActiveAction() {
		if (actionQueue.size() > 0) {
			return actionQueue.front()->GetValue();
		}
		return "NULL";
	}
private:
	Ref<Actor> owner;
	void ExecuteAction(Action* action);
	std::vector<Action*> actionQueue;
	float timer;
};

