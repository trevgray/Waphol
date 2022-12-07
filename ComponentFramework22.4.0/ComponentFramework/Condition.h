#pragma once
#include "Actor.h"

class Condition {
protected:
	Ref<Actor> owner;
public:
	Condition() { owner = nullptr; }
	Condition(Ref<Actor> owner_) { owner = owner_; }
	~Condition() {}
	virtual bool Test() { return false; }
	void SetOwner(Ref<Actor> owner_) { owner = owner_; }
};