#pragma once
#include <unordered_map>
#include "Actor.h"

#include "NavigationMesh.h"
class ActorManager {
private:
	std::unordered_map <std::string, Ref<Actor>> actorGraph;

	std::vector<Actor*> parentPointers;

	Ref<NavigationMesh> navMesh;
public:
	ActorManager();
	~ActorManager();

	void RemoveAllActors();

	int GetActorGraphSize() const;

	std::unordered_map<std::string, Ref<Actor>> GetActorGraph() const;

	bool AddParentPointer(Actor* parentActor);

	void LoadNonPrehabActors();
	void UpdateActors(const float deltaTime);
	void RenderActors(std::vector<std::string> shaders) const;

	template<typename ActorTemplate, typename ...Args> void AddActor(std::string name, Args && ...args_);

	void RemoveActor(std::string name);

	template<typename ActorTemplate> Ref<ActorTemplate> GetActor() const;
	template<typename ActorTemplate> Ref<ActorTemplate> GetActor(std::string name);

	//Nav Mesh
	Ref<NavigationMesh> GetNavigationMesh() { return navMesh; }
	void SetNavigationMesh(Ref<NavigationMesh> navMesh_) { navMesh = navMesh_; }
};