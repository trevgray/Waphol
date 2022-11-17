#include "ShaderComponent.h"
#include "Debug.h"
#include "EngineManager.h"
#include <fstream>
#include <string.h>

ShaderComponent::ShaderComponent(Component* parent_, std::string vsFilename_, std::string fsFilename_):
	Component(parent_),
	shaderID(0),vertShaderID(0),fragShaderID(0) {
	vsFilename = vsFilename_; //vert shader
	fsFilename = fsFilename_; //frag shader
}

ShaderComponent::~ShaderComponent() {
	OnDestroy();
}


bool ShaderComponent::OnCreate() {
	EngineManager::Instance()->GetRenderer()->CreateShader(vsFilename, fsFilename, shaderID, vertShaderID, fragShaderID, uniformMap);
	isCreated = true;
	return isCreated;
}

void ShaderComponent::OnDestroy() {
	EngineManager::Instance()->GetRenderer()->DeleteShader(shaderID, vertShaderID, fragShaderID);
	isCreated = false;
}

unsigned int ShaderComponent::GetUniformID(std::string name) {
	auto id = uniformMap.find(name);
#ifdef _DEBUG
	if (id == uniformMap.end()) {
		///Debug::Error(name + " :No such uniform name", __FILE__, __LINE__);
		return -1;
	}
#endif 
	return id->second;
}

/// Not used, but must be made concrete
void ShaderComponent::Update(const float deltaTime) {}
void ShaderComponent::Render() const {}