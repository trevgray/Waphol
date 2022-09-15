#pragma once

#include "Actor.h"
#include "AssetManager.h"
#include "CameraActor.h"
#include "LightActor.h"
#include <unordered_map>

union SDL_Event;

class Scene {
private:
	Ref<AssetManager> assetManager;
	std::unordered_map <std::string, Ref<Actor>> actorGraph;
public:
	virtual ~Scene() {};
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
	}

	void RemovePointer(Actor* removeActor) {
		delete removeActor;
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

	void LoadNonPrehabActors() {
		for (auto& component : assetManager->GetComponentGraph()) {
			Actor* actor = dynamic_cast<Actor*>(component.second.get());
			if (actor != nullptr && actor->getPrehab() == false) {
				AddActor<Actor>(component.first, new Actor(nullptr));
				GetActor<Actor>(component.first)->InheritActor(assetManager->GetComponent<Actor>(component.first.c_str()));
				GetActor<Actor>(component.first)->OnCreate(); 
			}
			actor = dynamic_cast<CameraActor*>(component.second.get());
			if (actor != nullptr && actor->getPrehab() == false) {
				AddActor<CameraActor>(component.first, new CameraActor(nullptr));
				GetActor<CameraActor>()->InheritActor(assetManager->GetComponent<Actor>(component.first.c_str()));
				GetActor<CameraActor>()->OnCreate();
			}
			actor = dynamic_cast<LightActor*>(component.second.get());
			if (actor != nullptr && actor->getPrehab() == false) {
				AddActor<LightActor>(component.first, new CameraActor(nullptr));
				GetActor<LightActor>()->InheritActor(assetManager->GetComponent<Actor>(component.first.c_str()));
				GetActor<LightActor>()->OnCreate();
			}
		}
	}

	void LoadAssetManager(std::string XMLFile_, std::string SceneName_) {
		assetManager = std::make_shared<AssetManager>();
		assetManager->BuildSceneAssets(XMLFile_, SceneName_);
		assetManager->OnCreate();
	}

	Ref<AssetManager> GetAssetManager() const { return assetManager; }



	//template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(int objectNum) const { //old version that used array indices to get specific actors
	//	if (dynamic_cast<ComponentTemplate*>(actorGraph[objectNum].get())) { //check if it is the type we want
	//		return std::dynamic_pointer_cast<ComponentTemplate>(actorGraph[objectNum]);
	//	}
	//	return nullptr;
	//}
};