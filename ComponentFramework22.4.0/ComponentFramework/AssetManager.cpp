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
#include "PhysicsBodyComponent.h"
#include "ControllerComponent.h"
#include "SteeringComponent.h"

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

void AssetManager::BuildSceneAssets(std::string XMLFile_, std::string SceneName_) { //TODO - Steering Component
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
	/*for (const tinyxml2::XMLAttribute* a = currentElement->FirstChildElement("Shader")->FirstAttribute(); a; a = a->Next()) { //loop through all the attributes
		std::cout << a->Value() << std::endl; - save for maybe asset manager redesign?
	}*/

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
			else if (componentType == "Controller") { //create controller component
				AddComponent<ControllerComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Controller")->Attribute("controllerTemplate"));
			}
			else if (componentType == "Shader") { //create shader component
				AddComponent<ShaderComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Shader")->Attribute("vertFileName"), currentElement->FirstChildElement("Shader")->Attribute("fragFileName"));
			}
			else if (componentType == "PhysicsBody") { //create physics component
				BuildPhysicsBodyComponent();
			}
			else if (componentType == "Shape") { //create shape component
				BuildShapeComponent();
			}
			else if (componentType == "Steering") { //create shape component
				BuildSteeringComponent();
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
				GetComponent<Actor>(currentElement->Attribute("name"))->SetPrehab(false); //set the object to not be a prehab because it has a custom transform
			}
			if (currentElement->FirstChildElement("Shape") != nullptr) {
				currentComponent = currentElement->FirstChildElement("Shape");
				GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<ShapeComponent>(GetComponent<ShapeComponent>(currentComponent->Attribute("name")));
			}
			if (currentElement->FirstChildElement("PhysicsBody") != nullptr) {
				currentComponent = currentElement->FirstChildElement("PhysicsBody");
				GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<PhysicsBodyComponent>(GetComponent<PhysicsBodyComponent>(currentComponent->Attribute("name")));
			}
			if (currentElement->FirstChildElement("Controller") != nullptr) {
				currentComponent = currentElement->FirstChildElement("Controller");
				GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<ControllerComponent>(GetComponent<ControllerComponent>(currentComponent->Attribute("name")));
			}
			if (currentElement->FirstChildElement("Steering") != nullptr) {
				currentComponent = currentElement->FirstChildElement("Steering");
				GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<SteeringComponent>(GetComponent<SteeringComponent>(currentComponent->Attribute("name")));
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
	else if (currentElement->FirstChildElement("Box") != nullptr) {
		tinyxml2::XMLElement* Box = currentElement->FirstChildElement("Box");
		AddComponent<ShapeComponent>(currentElement->Attribute("name"), nullptr, GEOMETRY::Box(
			Vec3(Box->FloatAttribute("centerX"), Box->FloatAttribute("centerY"), Box->FloatAttribute("centerZ")),
			Vec3(Box->FloatAttribute("halfExtentX"), Box->FloatAttribute("halfExtentY"), Box->FloatAttribute("halfExtentZ")), 
			QMath::angleAxisRotation(Box->FloatAttribute("angleDeg"), Vec3(Box->FloatAttribute("axisX"), Box->FloatAttribute("axisY"), Box->FloatAttribute("axisZ")))));
	}
}

void AssetManager::BuildPhysicsBodyComponent() {
	AddComponent<PhysicsBodyComponent>(currentElement->Attribute("name"), nullptr,
		Vec3(currentElement->FirstChildElement("Velocity")->FloatAttribute("x"), currentElement->FirstChildElement("Velocity")->FloatAttribute("y"), currentElement->FirstChildElement("Velocity")->FloatAttribute("z")), /*velocity*/
		Vec3(currentElement->FirstChildElement("Acceleration")->FloatAttribute("x"), currentElement->FirstChildElement("Acceleration")->FloatAttribute("y"), currentElement->FirstChildElement("Acceleration")->FloatAttribute("z")), /*accel*/
		currentElement->FirstChildElement("Attributes")->FloatAttribute("mass"),
		currentElement->FirstChildElement("Attributes")->FloatAttribute("maxSpeed"),
		currentElement->FirstChildElement("Attributes")->FloatAttribute("maxAcceleration"),
		currentElement->FirstChildElement("Attributes")->FloatAttribute("maxRotation"),
		currentElement->FirstChildElement("Attributes")->FloatAttribute("maxAngular"));
}

void AssetManager::BuildSteeringComponent() {
	std::vector<std::string> steeringBehaviourNames; //set up arrays for all the names of the behaviours & arguments
	std::vector<std::string> steeringArguments;
	std::vector<float> weights;
	for (const tinyxml2::XMLAttribute* a = currentElement->FirstChildElement("SteeringBehaviour")->FirstAttribute(); a; a = a->Next()) { //loop through all the attributes
		steeringBehaviourNames.push_back(a->Value()); //add all the names of the behaviours
	}
	for (const tinyxml2::XMLAttribute* a = currentElement->FirstChildElement("Arguments")->FirstAttribute(); a; a = a->Next()) { //loop through all the attributes
		steeringArguments.push_back(a->Value()); //add all the arguments
	}
	for (const tinyxml2::XMLAttribute* a = currentElement->FirstChildElement("Weights")->FirstAttribute(); a; a = a->Next()) { //loop through all the attributes
		weights.push_back(a->FloatValue()); //add all the floats
	}
	size_t steeringArgumentsIterator = 0; //set an iterator to loop through all the arguments between the behaviours
	size_t weightIterator = 0;
	std::vector<Ref<SteeringBehaviour>> steeringBehaviours;
	for (std::string behaviour : steeringBehaviourNames) {
		if (behaviour == "Align") {
			steeringBehaviours.push_back(std::make_shared<Align>(steeringArguments[steeringArgumentsIterator]));
			steeringArgumentsIterator += 1;
		}
		else if (behaviour == "Arrive") {
			steeringBehaviours.push_back(std::make_shared<Arrive>(steeringArguments[steeringArgumentsIterator], 
				std::stof(steeringArguments[steeringArgumentsIterator + 1]),
				std::stof(steeringArguments[steeringArgumentsIterator + 2]),
				std::stof(steeringArguments[steeringArgumentsIterator + 3])));
			steeringArgumentsIterator += 4;
		}
		else if (behaviour == "FaceVelocity") {
			steeringBehaviours.push_back(std::make_shared<FaceVelocity>(steeringArguments[steeringArgumentsIterator]));
			steeringArgumentsIterator += 1;
		}
		else if (behaviour == "Flee") {
			steeringBehaviours.push_back(std::make_shared<Flee>(steeringArguments[steeringArgumentsIterator]));
			steeringArgumentsIterator += 1;
		}
		else if (behaviour == "Seek") {
			steeringBehaviours.push_back(std::make_shared<Seek>(steeringArguments[steeringArgumentsIterator]));
			steeringArgumentsIterator += 1;
		}
		else if (behaviour == "VelocityMatch") {
			steeringBehaviours.push_back(std::make_shared<VelocityMatch>(steeringArguments[steeringArgumentsIterator],
				std::stof(steeringArguments[steeringArgumentsIterator + 1])));
			steeringArgumentsIterator += 2;
		}
		steeringBehaviours.back()->SetWeight(weights[weightIterator]);
		weightIterator++;
	}

	AddComponent<SteeringComponent>(currentElement->Attribute("name"), nullptr, steeringBehaviours);
}