#pragma once
#include <unordered_map>
#include "Actor.h"
class ActorManager {
private:
	std::unordered_map <std::string, Ref<Actor>> actorGraph;
public:
	ActorManager();
	~ActorManager();

	void RemoveAllComponents();

	int GetActorGraphSize() const;

	std::unordered_map<std::string, Ref<Actor>> GetActorGraph() const;

	void LoadNonPrehabActors();
	void RenderActors(std::vector<std::string> shaders) const;
	void LoadAssets(std::string XMLFile_, std::string SceneName_);

	template<typename ActorTemplate, typename ...Args> void AddActor(std::string name, Args && ...args_);

	template<typename ActorTemplate> Ref<ActorTemplate> GetActor() const;
	template<typename ActorTemplate> Ref<ActorTemplate> GetActor(std::string name);
};