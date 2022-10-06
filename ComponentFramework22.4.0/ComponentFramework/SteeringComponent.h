#pragma once
#include "Component.h"
#include <vector>
#include "SteeringBehaviour.h"

#include "Seek.h"
#include "VelocityMatch.h"
#include "Arrive.h"
#include "Align.h"
#include "Flee.h"
#include "FaceVelocity.h"
#include "EngineManager.h"

class SteeringComponent : public Component {
	SteeringComponent(const SteeringComponent&) = delete;
	SteeringComponent(SteeringComponent&&) = delete;
	SteeringComponent& operator = (const SteeringComponent&) = delete;
	SteeringComponent& operator = (SteeringComponent&&) = delete;
public:
	SteeringComponent(Component* parent_, std::vector<Ref<SteeringBehaviour>> steeringBehaviours_, Ref<Actor> actor_);
	SteeringComponent(Component* parent_, Ref<SteeringBehaviour> steerBehaviour_, Ref<Actor> actor_);

	SteeringComponent(Component* parent_, std::vector<Ref<SteeringBehaviour>> steeringBehaviours_);

	virtual ~SteeringComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	void RunSteering();
	virtual void Render() const;

private:
	std::vector<Ref<SteeringBehaviour>> steeringBehaviours;
	SteeringOutput steering;
	Ref<Actor> actorComponent;
};

