#include "State.h"
#include "Transition.h"

State::~State() {
	transitions.clear();
	if (action) { delete action; }
	/*for (Transition* transition : transitions) {
		if (transition) { delete transition; }
	}
	transitions.clear();*/
}

void State::AddTransition(Transition transition) {
	transitions.push_back(transition);
}
