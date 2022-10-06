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
	//Component Graph
	std::unordered_map <std::string, Ref<Component>> componentGraph;
	//XML Elements
	tinyxml2::XMLElement* sceneRoot; 
	tinyxml2::XMLElement* currentElement;
	//Lists of potential components and actors types in a scene
	//std::vector<std::string> componentTypes; //idea for the future - loop through all the component types in the BuildComponents() and BuildActors() check the currentComponent - then add them - use XML to add the componentTypes
	//Build Component Functions
	void BuildSceneAssets(std::string XMLFile_, std::string SceneName_);
	void BuildCameraActor();
	void BuildLightActors();
	void BuildComponents();
	void BuildActors();

	void BuildShapeComponent();
	void BuildPhysicsBodyComponent();
	void BuildSteeringComponent();
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

