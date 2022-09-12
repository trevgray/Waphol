#include "Actor.h"
#include "Debug.h"
#include "MeshComponent.h"
Actor::Actor(Component* parent_):Component(parent_) {}

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
		if (component->OnCreate() == false) {
			Debug::Error("Loading assets for Actor/Components: ", __FILE__, __LINE__);
			isCreated = false;
			return isCreated;
		}
	}
	isCreated = true;
	return isCreated;
}

void Actor::Update(const float deltaTime) {
	
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
		modelMatrix = dynamic_cast<Actor*>(parent)->GetModelMatrix() * modelMatrix;
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
	for (auto component : inheritActor->GetComponentVector()) {
		components.push_back(component);
	}
}