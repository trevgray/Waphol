#ifndef SCENE_H
#define SCENE_H

#include "Actor.h"
#include "LightActor.h"
#include <unordered_map>

union SDL_Event;

class Scene{
private:
	//std::vector<Ref<Component>> actors; //make an unordered map
	std::unordered_map <std::string, Ref<Actor>> actorGraph;
	std::vector<Actor*> emptyActors;
	//AssetManager* assetManager;
public:
	//std::unordered_map <std::string, Ref<Actor>> actorGraph;
	virtual ~Scene() {
		for (Actor* actor : emptyActors) {
			delete actor;
		}
		emptyActors.clear();
	}
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;

	//AssetManager* GetAssetManager() const { return assetManager; }
	
	int GetActorGraphSize() const { return actorGraph.size(); }

	std::unordered_map <std::string, Ref<Actor>> GetActorGraph() const{ return actorGraph; }

	template<typename ActorTemplate, typename ... Args> void AddActor(std::string name, Args&& ... args_) { //rename to AddActor - also use actortemplate
		Ref<ActorTemplate> t = std::make_shared<ActorTemplate>(std::forward<Args>(args_)...);
		actorGraph[name] = t;
		RemovePointer(std::forward<Args>(args_)...);
		//write a function that removes the pointer
	}

	void RemovePointer(Actor* removeActor) {
		emptyActors.push_back(removeActor);
		//std::cout << "  " << removeActor << std::endl;
	}

	template<typename ActorTemplate> Ref<ActorTemplate> GetActor() const { //for compatibility with older scenes
		for (auto actor : actorGraph) {
			if (dynamic_cast<ActorTemplate*>(actor.second.get())) {
				//https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast dynamic cast designed for shared_ptr's
				return std::dynamic_pointer_cast<ActorTemplate>(actor.second);
			}
		}
		return Ref<ActorTemplate>(nullptr);
	}

	template<typename ActorTemplate> Ref<ActorTemplate> GetActor(std::string name) {
		auto id = actorGraph.find(name);
#ifdef _DEBUG
		if (id == actorGraph.end()) {
			Debug::Error("Can't find requested component", __FILE__, __LINE__);
			return Ref<ActorTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ActorTemplate>(id->second);
	}

	//template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(int objectNum) const { //old version that used array indices to get specific actors
	//	if (dynamic_cast<ComponentTemplate*>(actorGraph[objectNum].get())) { //check if it is the type we want
	//		return std::dynamic_pointer_cast<ComponentTemplate>(actorGraph[objectNum]);
	//	}
	//	return nullptr;
	//}
};
#endif