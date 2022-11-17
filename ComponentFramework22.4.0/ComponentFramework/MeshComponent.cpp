#include "MeshComponent.h"

#include "EngineManager.h"

using namespace MATH;

MeshComponent::MeshComponent(Component* parent_, std::string filename_) : Component(parent_) {
    filename = filename_;
}
MeshComponent::~MeshComponent() {
    OnDestroy();
}

bool MeshComponent::OnCreate() {
    if (isCreated) return isCreated;
    LoadModel(filename.c_str());
    //StoreMeshData(GL_TRIANGLES);
    isCreated = true;
    return isCreated;
}



void MeshComponent::LoadModel(const char* filename) {
    EngineManager::Instance()->GetRenderer()->LoadModel(filename, vao, vbo, dateLength);
}

void MeshComponent::Render() const {
    EngineManager::Instance()->GetRenderer()->RenderModel(vao, dateLength, 4); //4 is GL_TRIANGLES
}

void MeshComponent::Render(unsigned int drawmode_) const {
    EngineManager::Instance()->GetRenderer()->RenderModel(vao, dateLength, drawmode_);
}

void MeshComponent::OnDestroy() {
    EngineManager::Instance()->GetRenderer()->DeleteBuffers(1, vbo);
    EngineManager::Instance()->GetRenderer()->DeleteVertexArrays(1, vao);
    isCreated = false;
}

/// Currently unused.

void MeshComponent::Update(const float deltaTime) {}