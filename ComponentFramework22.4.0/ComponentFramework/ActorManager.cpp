#include "ActorManager.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "EngineManager.h"

ActorManager::ActorManager() {
}

ActorManager::~ActorManager() {
	RemoveAllComponents();
}

void ActorManager::RemoveAllComponents() {
	actorGraph.clear();
}

int ActorManager::GetActorGraphSize() const { return actorGraph.size(); }

std::unordered_map <std::string, Ref<Actor>> ActorManager::GetActorGraph() const { return actorGraph; }

template<typename ActorTemplate, typename ... Args> void ActorManager::AddActor(std::string name, Args&& ... args_) {
	Ref<ActorTemplate> t = std::make_shared<ActorTemplate>(std::forward<Args>(args_)...);
	actorGraph[name] = t;
	RemovePointer(std::forward<Args>(args_)...);
}

void RemovePointer(Actor* removeActor) {
	delete removeActor;
	//std::cout << "  " << removeActor << std::endl;
}

template<typename ActorTemplate> Ref<ActorTemplate> ActorManager::GetActor() const { //for compatibility with older scenes
	for (auto actor : actorGraph) {
		if (dynamic_cast<ActorTemplate*>(actor.second.get())) {
			//https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast dynamic cast designed for shared_ptr's
			return std::dynamic_pointer_cast<ActorTemplate>(actor.second);
		}
	}
	return Ref<ActorTemplate>(nullptr);
}

template<typename ActorTemplate> Ref<ActorTemplate> ActorManager::GetActor(std::string name) {
	auto id = actorGraph.find(name);
#ifdef _DEBUG
	if (id == actorGraph.end()) {
		Debug::Error("Can't find requested component", __FILE__, __LINE__);
		return Ref<ActorTemplate>(nullptr);
	}
#endif
	return std::dynamic_pointer_cast<ActorTemplate>(id->second);
}

void ActorManager::LoadNonPrehabActors() {
	for (auto& component : EngineManager::Instance()->GetAssetManager()->GetComponentGraph()) {
		Actor* actor = dynamic_cast<Actor*>(component.second.get());
		if (actor != nullptr && actor->getPrehab() == false) {
			AddActor<Actor>(component.first, new Actor(nullptr));
			GetActor<Actor>(component.first)->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(component.first.c_str()));
			GetActor<Actor>(component.first)->OnCreate();
		}
		actor = dynamic_cast<CameraActor*>(component.second.get());
		if (actor != nullptr && actor->getPrehab() == false) {
			AddActor<CameraActor>(component.first, new CameraActor(nullptr));
			GetActor<CameraActor>()->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(component.first.c_str()));
			GetActor<CameraActor>()->OnCreate();
		}
		actor = dynamic_cast<LightActor*>(component.second.get());
		if (actor != nullptr && actor->getPrehab() == false) {
			AddActor<LightActor>(component.first, new CameraActor(nullptr));
			GetActor<LightActor>()->InheritActor(EngineManager::Instance()->GetAssetManager()->GetComponent<Actor>(component.first.c_str()));
			GetActor<LightActor>()->OnCreate();
		}
	}
}

void ActorManager::RenderActors(std::vector<std::string> shaders) const {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, GetActor<LightActor>()->GetLightID());

	for (std::string shaderFileName : shaders) {
		glUseProgram(EngineManager::Instance()->GetAssetManager()->GetComponent<ShaderComponent>(shaderFileName.c_str())->GetProgram());

		for (auto actor : GetActorGraph()) {
			glUniformMatrix4fv(EngineManager::Instance()->GetAssetManager()->GetComponent<ShaderComponent>(shaderFileName.c_str())->GetUniformID("modelMatrix"), 1, GL_FALSE, actor.second->GetModelMatrix());
			if (actor.second->GetComponent<MaterialComponent>() != nullptr) { //everything is an actor, so i just check if it has a texture
				glBindTexture(GL_TEXTURE_2D, actor.second->GetComponent<MaterialComponent>()->getTextureID()); //this is also amazing because we can add as many actors as we want, and the render does not need to change
				actor.second->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
}

//template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(int objectNum) const { //old version that used array indices to get specific actors
//	if (dynamic_cast<ComponentTemplate*>(actorGraph[objectNum].get())) { //check if it is the type we want
//		return std::dynamic_pointer_cast<ComponentTemplate>(actorGraph[objectNum]);
//	}
//	return nullptr;
//}