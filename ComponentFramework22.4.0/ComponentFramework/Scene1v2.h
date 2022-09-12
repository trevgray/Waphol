#pragma once
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "Actor.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene1v2 : public Scene { //, public Actor { //inherited from actor, but i'm not really using any of actor's functions
private:
	int nextRow;
	float RowX, RowY;
	//std::vector<std::shared_ptr<Component>> components;
public:
	explicit Scene1v2();
	virtual ~Scene1v2();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);

	//Overwriting the AddComponent and GetComponent functions in Actor so more than 1 type of Actor can be added to the Scene

	//template<typename ComponentTemplate, typename ... Args> void AddComponent(Args&& ... args_) {
	//	components.push_back(std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...));
	//}

	//template<typename ComponentTemplate> void AddComponent(Ref<ComponentTemplate> component_) {
	//	components.push_back(component_);
	//}

	//template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent() const {
	//	for (auto c : components) {
	//		if (dynamic_cast<ComponentTemplate*>(c.get())) {
	//			//https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast dynamic cast designed for shared_ptr's
	//			return std::dynamic_pointer_cast<ComponentTemplate>(c);
	//		}
	//	}
	//	return Ref<ComponentTemplate>(nullptr);
	//}

	//template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(int objectNum) const {
	//	if (dynamic_cast<ComponentTemplate*>(components[objectNum].get())) { //check if it is the type we want
	//		return std::dynamic_pointer_cast<ComponentTemplate>(components[objectNum]);
	//	}
	//	return nullptr;
	//}
};