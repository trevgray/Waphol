#pragma once
#include <list>

#include "Action.h"

//#include "Transition.h";
class Transition;

enum class STATE { SEEK = 0, DO_NOTHING };

class State {
private:
	Action action;
	std::list<Transition> transitions;
	//ignoring Actions for this simple example
	STATE name;
public:
	State(STATE name_, std::string actionSet_) : action(actionSet_) { name = name_; }
	~State();
	std::list<Transition> GetTransitions() { return transitions; }
	void AddTransition(Transition transition);
	STATE GetName() { return name; }
	Action GetAction() { return action; }
};

