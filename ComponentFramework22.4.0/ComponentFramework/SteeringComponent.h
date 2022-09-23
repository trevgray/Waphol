#pragma once
#include "Component.h"
#include <vector>
#include "SteeringBehaviour.h"

class SteeringComponent : public Component {
	SteeringComponent(const SteeringComponent&) = delete;
	SteeringComponent(SteeringComponent&&) = delete;
	SteeringComponent& operator = (const SteeringComponent&) = delete;
	SteeringComponent& operator = (SteeringComponent&&) = delete;
public:
	SteeringComponent(Component* parent_, std::vector<SteeringBehaviour> steeringBehaviours_, Ref<Actor> actor_);
	virtual ~SteeringComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	void RunSteering();
	virtual void Render() const;

private:
	std::vector<SteeringBehaviour> steeringBehaviours;
	SteeringOutput steering;
	Ref<Actor> actorComponent;
};

