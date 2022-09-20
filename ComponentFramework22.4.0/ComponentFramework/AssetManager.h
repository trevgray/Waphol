#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include "Component.h"
#include "Actor.h"
#include "Debug.h"
#include "tinyxml2.h"

class AssetManager {
private:
	std::unordered_map <std::string, Ref<Component>> componentGraph;
	tinyxml2::XMLElement* sceneRoot; //getting root of the scene
	tinyxml2::XMLElement* currentElement;

	void BuildCameraActor();
	void BuildLightActors();
	void BuildComponents();
	void BuildActors();
	void BuildSceneAssets(std::string XMLFile_, std::string SceneName_);
	
	void BuildShapeComponent();
public:
	AssetManager();
	~AssetManager();
	bool OnCreate();
	void LoadAssets(std::string XMLFile_, std::string SceneName_);
	void RemoveAllComponents();

	template<typename ComponentTemplate, typename ... Args> void AddComponent(std::string name, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		componentGraph[name] = t;
	}

	template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(const char* name) {
		auto id = componentGraph.find(name);
#ifdef _DEBUG
		if (id == componentGraph.end()) {
			Debug::Error("Can't find requested component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ComponentTemplate>(id->second);
	}

	std::unordered_map <std::string, Ref<Component>> GetComponentGraph() { return componentGraph; }

	//template<typename ComponentTemplate> void GenerateSingleArgumentAsset(std::string name, std::string filePath) { //there is probably a way to use one GenerateAsset method for all the assets with typename ... Args, but I don't want to do it right now
	//	AddComponent<ComponentTemplate>(name, nullptr, filePath); //you could probably could read the # of arugments and AddComponent using ifs
	//}

	//template<typename ComponentTemplate> void GenerateDoubleArgumentAsset(std::string name, std::string filePath1, std::string filePath2) {
	//	AddComponent<ComponentTemplate>(name, nullptr, filePath1, filePath2);
	//}
};

