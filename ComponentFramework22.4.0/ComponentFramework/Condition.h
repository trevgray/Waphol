#pragma once
#include "Actor.h"
//abstract class (has a pure virtual function)
class Condition {
protected:
	Ref<Actor> owner;
public:
	Condition(Ref<Actor> owner_) { owner = owner_; }
	~Condition() {}
	virtual bool Test() = 0;
};