#include <memory>
#include "AssetManager.h"
#include "QMath.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "ShapeComponent.h"
#include "CameraActor.h"
#include "LightActor.h"

AssetManager::AssetManager() : sceneRoot(nullptr), currentElement(nullptr) {}

AssetManager::~AssetManager() {
	RemoveAllComponents();
}

void AssetManager::LoadAssets(std::string XMLFile_, std::string SceneName_) {
	BuildSceneAssets(XMLFile_, SceneName_);
	OnCreate();
}

bool AssetManager::OnCreate() {
	for (std::pair<std::string, Ref<Component>> c : componentGraph) {
		if (c.second->OnCreate() == false) {
#ifdef _DEBUG
			Debug::Error("Component failed to create", __FILE__, __LINE__);
#endif
			return false;
		}
	}
	return true;
}

void AssetManager::RemoveAllComponents() {
	componentGraph.clear();
}

void AssetManager::BuildSceneAssets(std::string XMLFile_, std::string SceneName_) { //TODO - Controller Component, PhysicsBodyComponent
	tinyxml2::XMLDocument XMLFile;
	XMLFile.LoadFile(XMLFile_.c_str()); //loading XML file
	if (XMLFile.Error()) { //Error detection in the xml
		std::cout << XMLFile.ErrorIDToName(XMLFile.ErrorID()) << std::endl;
		return;
	}
	sceneRoot = XMLFile.RootElement()->FirstChildElement(SceneName_.c_str());

	BuildCameraActor();
	BuildLightActors();
	BuildComponents();
	BuildActors();
	
	//set everything to null
	sceneRoot = nullptr; currentElement = nullptr;
}

void AssetManager::BuildCameraActor() {
	//Camera Component
	currentElement = sceneRoot->FirstChildElement("Camera"); //loading the camera
	AddComponent<CameraActor>(currentElement->Attribute("name"), nullptr); //create the camera actor
	tinyxml2::XMLElement* transformElement = currentElement->FirstChildElement("Transform"); //i did this for you to read, it is unnecessary
	GetComponent<CameraActor>(currentElement->Attribute("name"))->AddComponent<TransformComponent>(nullptr, //make transform component
		Vec3(transformElement->FloatAttribute("posx"), transformElement->FloatAttribute("posy"), transformElement->FloatAttribute("posz")), //position
		//Quaternion(transformElement->FloatAttribute("qw"), transformElement->FloatAttribute("qx"), transformElement->FloatAttribute("qy"), transformElement->FloatAttribute("qz"))); //quaternion
		QMath::angleAxisRotation(transformElement->FloatAttribute("angleDeg"), Vec3(transformElement->FloatAttribute("axisx"), transformElement->FloatAttribute("axisy"), transformElement->FloatAttribute("axisz")))); //using angle Axis Rotation then converting to Quaternions
}

void AssetManager::BuildLightActors() {
	//Light Loop - in Scene Scope
	currentElement = sceneRoot->FirstChildElement("Light"); //loading first component from Scene Scope
	bool lightLoop = true;
	while (lightLoop) {
		std::string lightCheck = currentElement->Name();
		if (lightCheck == "Light") { //check if the element is a component
			//Light loop
			tinyxml2::XMLElement* locationElement = currentElement->FirstChildElement("Location"); //i did this for you to read, these are unnecessary
			tinyxml2::XMLElement* colourElement = currentElement->FirstChildElement("Colour"); //but imagine if I didn't do it
			tinyxml2::XMLElement* falloutElement = currentElement->FirstChildElement("Falloff");
			AddComponent<LightActor>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("LightStyle")->Attribute("style"),
				Vec3(locationElement->FloatAttribute("x"), locationElement->FloatAttribute("y"), locationElement->FloatAttribute("z")), //location
				Vec4(colourElement->FloatAttribute("r"), colourElement->FloatAttribute("g"), colourElement->FloatAttribute("b"), colourElement->FloatAttribute("a")), //colour
				currentElement->FirstChildElement("Intensity")->FloatAttribute("Intensity"), //intensity
				Vec3(falloutElement->FloatAttribute("x"), falloutElement->FloatAttribute("y"), falloutElement->FloatAttribute("z"))); //fallout
			if (currentElement == sceneRoot->LastChildElement("Light")) { //stopping looping when the current element is the last element in Scene Scope - sceneRoot->LastChild() will also work, but stopping at the last component should be faster
				lightLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement(); //loading the next component
	}
}

void AssetManager::BuildComponents() {
	//Component Loop - in Scene Scope
	currentElement = sceneRoot->FirstChildElement("Component"); //loading first component from Scene Scope
	bool componentLoop = true;
	while (componentLoop) {
		std::string componentCheck = currentElement->Name();
		if (componentCheck == "Component") { //check if the element is a component
			//component loop
			std::string componentType = currentElement->FirstChildElement("Type")->Attribute("type");

			if (componentType == "Mesh") { //create mesh component
				AddComponent<MeshComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Mesh")->Attribute("filename"));
			}
			else if (componentType == "Material") { //create material component
				AddComponent<MaterialComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Material")->Attribute("filename"));
			}
			else if (componentType == "Shader") { //create shader component
				AddComponent<ShaderComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Shader")->Attribute("vertFileName"), currentElement->FirstChildElement("Shader")->Attribute("fragFileName"));
			}
			else if (componentType == "Shape") { //create shape component
				BuildShapeComponent();
			}
			if (currentElement == sceneRoot->LastChildElement("Component")) { //stopping looping when the current element is the last element in Scene Scope - sceneRoot->LastChild() will also work, but stopping at the last component should be faster
				componentLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement(); //loading the next component
	}
}

void AssetManager::BuildActors() {
	//Actor Loop
	currentElement = sceneRoot->FirstChildElement("Actor"); //find first actor = currentElement 
	tinyxml2::XMLElement* currentComponent;
	bool actorLoop = true;
	while (actorLoop) {
		std::string actorCheck = currentElement->Name();
		if (actorCheck == "Actor") { //check if the element is a actor
			Component* parentActor = nullptr;
			std::string parentCheck = currentElement->Attribute("parent");
			if (parentCheck != "null") { //check if the actor has a parent
				parentActor = GetComponent<Component>(currentElement->Attribute("parent")).get(); //set the parent of the actor
			}
			AddComponent<Actor>(currentElement->Attribute("name"), parentActor); //make the actor
			currentComponent = currentElement->FirstChildElement("Mesh"); //get the FirstChildElement component in currentElement
			GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<MeshComponent>(GetComponent<MeshComponent>(currentComponent->Attribute("name"))); //set the mesh to a component made in the first loop
			currentComponent = currentElement->FirstChildElement("Material");
			GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<MaterialComponent>(GetComponent<MaterialComponent>(currentComponent->Attribute("name"))); //set the material to a component made in the first loop
			if (currentElement->FirstChildElement("Transform") != nullptr) { //if the transform component exists - I don't always want to give every actor a transform, so i did this
				tinyxml2::XMLElement* transformElement = currentElement->FirstChildElement("Transform"); //i did this for you to read, it is unnecessary
				GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<TransformComponent>(nullptr, //make transform component
					Vec3(transformElement->FloatAttribute("posx"), transformElement->FloatAttribute("posy"), transformElement->FloatAttribute("posz")), //position
					QMath::angleAxisRotation(transformElement->FloatAttribute("angleDeg"), Vec3(transformElement->FloatAttribute("axisx"), transformElement->FloatAttribute("axisy"), transformElement->FloatAttribute("axisz"))),
					//Quaternion(transformElement->FloatAttribute("qw"), transformElement->FloatAttribute("qx"), transformElement->FloatAttribute("qy"), transformElement->FloatAttribute("qz")),  //quaternions
					Vec3(transformElement->FloatAttribute("scalex"), transformElement->FloatAttribute("scaley"), transformElement->FloatAttribute("scalez"))); //scale
				GetComponent<Actor>(currentElement->Attribute("name"))->setPrehab(false); //set the object to not be a prehab because it has a custom transform
			}
			if (currentElement->FirstChildElement("Shape") != nullptr) {
				currentComponent = currentElement->FirstChildElement("Shape");
				GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<ShapeComponent>(GetComponent<ShapeComponent>(currentComponent->Attribute("name")));
			}
			if (currentElement == sceneRoot->LastChildElement("Actor")) { //exit when component is = to the LastChildElement in currentComponent
				actorLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement(); //load next actor
	}
}

void AssetManager::BuildShapeComponent() {
	if (currentElement->FirstChildElement("Sphere") != nullptr) {
		tinyxml2::XMLElement* Sphere = currentElement->FirstChildElement("Sphere");
		AddComponent<ShapeComponent>(currentElement->Attribute("name"), nullptr, GEOMETRY::Sphere(Sphere->FloatAttribute("centreX"),
			Sphere->FloatAttribute("centreY"), Sphere->FloatAttribute("centreZ"), Sphere->FloatAttribute("radius")));
		//Sphere = nullptr;
	}
	else if (currentElement->FirstChildElement("Cylinder") != nullptr) {
		tinyxml2::XMLElement* Cylinder = currentElement->FirstChildElement("Cylinder");
		AddComponent<ShapeComponent>(currentElement->Attribute("name"), nullptr, GEOMETRY::Cylinder(Cylinder->FloatAttribute("radius"),
			Vec3(Cylinder->FloatAttribute("topX"), Cylinder->FloatAttribute("topY"), Cylinder->FloatAttribute("topZ")), 
			Vec3(Cylinder->FloatAttribute("bottomX"), Cylinder->FloatAttribute("bottomY"), Cylinder->FloatAttribute("bottomZ"))));
	}
	else if (currentElement->FirstChildElement("Capsule") != nullptr) {
		tinyxml2::XMLElement* Capsule = currentElement->FirstChildElement("Capsule");
		AddComponent<ShapeComponent>(currentElement->Attribute("name"), nullptr, GEOMETRY::Capsule(Capsule->FloatAttribute("radius"),
			Vec3(Capsule->FloatAttribute("topSphereX"), Capsule->FloatAttribute("topSphereY"), Capsule->FloatAttribute("topSphereZ")),
			Vec3(Capsule->FloatAttribute("bottomSphereX"), Capsule->FloatAttribute("bottomSphereY"), Capsule->FloatAttribute("bottomSphereZ"))));
	}
}
