#pragma once
#include "Actor.h"

//abstract class
//#include "Character.h"

//class Character;

class DecisionTreeNode {
protected:
	Ref<Actor> owner;
public:
	virtual DecisionTreeNode* MakeDecision() = 0;
};