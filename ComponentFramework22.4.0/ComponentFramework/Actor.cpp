#include "Actor.h"
#include "Debug.h"
#include "MeshComponent.h"
Actor::Actor(Component* parent_):Component(parent_) {
	prehab = true;
	isCreated = false;
}

Actor::~Actor() {
	OnDestroy();
}

void Actor::OnDestroy() {
	RemoveAllComponents();
	//Debug::Info("Deleting assets for Actor: ", __FILE__, __LINE__);
	isCreated = false;
}

bool Actor::OnCreate() {
	if (isCreated) return isCreated;
	for (auto component : components) {
		if (component->GetCreated() == false) {
			component->OnCreate();
		}
		//OLD SYSTEM WHERE IT WAS NOT NORMAL FOR COMPONENTS TO RETURN FALSE
		/*if (component->OnCreate() == false) {
			Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
			isCreated = false;
			return isCreated;
		}*/
	}
	isCreated = true;
	return isCreated;
}

void Actor::Update(const float deltaTime) {
	for (auto component : components) {
		component->Update(deltaTime);
	}
}

void Actor::Render()const {}

Matrix4 Actor::GetModelMatrix() {
	Ref<TransformComponent> transform = GetComponent<TransformComponent>();
	if (transform.get()) {
		modelMatrix = transform->GetTransformMatrix();
	}
	else {
		modelMatrix.loadIdentity();
	}
	if (parent) {
		//if (dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>() != nullptr) {
		modelMatrix = dynamic_cast<Actor*>(parent)->GetModelMatrix() * modelMatrix;
		//}
	}
	return modelMatrix;
}

void Actor::RemoveAllComponents() {
	for (auto currentComponent : components) {
		currentComponent->OnDestroy();
		currentComponent.reset();
	}
	components.clear(); //clear the vector
}

void Actor::ListComponents() const {
	std::cout << typeid(*this).name() << " contains the following components:\n";
	for (auto component : components) {
		std::cout << typeid(*component).name() << std::endl;
	}
	std::cout << '\n';
}

void Actor::InheritActor(Ref<Actor> inheritActor) {
	if (parent) {
		parent = inheritActor.get()->parent;
	}
	for (auto component : inheritActor->GetComponentVector()) {
		components.push_back(component);
	}
}

void Actor::DeleteParent() {
	parent = nullptr;
}

Vec3 Actor::GetWorldPosition() {
	Vec3 pos;
	Ref<TransformComponent> transform = GetComponent<TransformComponent>();
	if (transform.get()) {
		pos = transform->GetPosition();
	}
	else {
		pos = Vec3();
	}
	if (parent) {
		//if (dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>() != nullptr) {
		pos = dynamic_cast<Actor*>(parent)->GetComponent<TransformComponent>()->GetPosition() + transform->GetPosition();
		//}
	}
	return pos;
}

//void Actor::SetParent(Ref<Actor>)