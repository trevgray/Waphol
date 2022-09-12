#pragma once
#include <vector> 
#include <iostream>
#include "Component.h"
#include "TransformComponent.h"
class Actor: public Component {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	Actor(const Actor&) = delete;
	Actor(Actor&&) = delete;
	Actor& operator=(const Actor&) = delete;
	Actor& operator=(Actor&&) = delete;

private:
	std::vector<Ref<Component>> components;
	Matrix4 modelMatrix;
public:
	Actor(Component* parent_);
	~Actor();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;

	template<typename ComponentTemplate, typename ... Args> void AddComponent(Args&& ... args_) {
		/// before you add the component, ask if you have the component in the list already,
		/// if so - don't add a second one. 
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		/// Using std::make_shared to do the work. This is the new idea!,
		components.push_back(std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...));
	}

	template<typename ComponentTemplate> void AddComponent(Ref<ComponentTemplate> component_) {
		if (GetComponent<ComponentTemplate>().get() != nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			return;
		}
		components.push_back(component_);
	}

	template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent() const {
		for (auto c : components) {
			if (dynamic_cast<ComponentTemplate*>(c.get())) {
				//https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast dynamic cast designed for shared_ptr's
				return std::dynamic_pointer_cast<ComponentTemplate>(c);
			}
		}
		return Ref<ComponentTemplate>(nullptr);
	}

	template<typename ComponentTemplate> ComponentTemplate* GetComponentRawPointer() const { //just for compatibility with the old code - not recommended to use
		for (std::shared_ptr<Component> c : components) {
			if (dynamic_cast<ComponentTemplate*>(c.get())) {
				return dynamic_cast<ComponentTemplate*>(c.get());
			}
		}
		return nullptr;
	}

	template<typename ComponentTemplate>
	void RemoveComponent() {
		for (unsigned int i = 0; i < components.size(); i++) {
			if (dynamic_cast<ComponentTemplate*>(components[i].get()) != nullptr) {
				/*components[i]->OnDestroy(); //not need for smart pointers
				delete components[i];
				components[i] = nullptr;
				//This removes the component from the vector list*/
				components.erase(components.begin() + i);
				break;
			}
		}
	}

	std::vector<Ref<Component>> GetComponentVector() { return components; }

	void RemoveAllComponents();
	void ListComponents() const;
	void InheritActor(Ref<Actor> inheritActor);
	Matrix4 GetModelMatrix();
	void setModelMatrix(Matrix4 modelMatrix_) { modelMatrix = modelMatrix_; }
};