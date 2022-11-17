#pragma once
#include "Component.h"
#include <unordered_map>

class ShaderComponent: public Component {
	ShaderComponent(const ShaderComponent&) = delete;
	ShaderComponent(ShaderComponent&&) = delete;
	ShaderComponent& operator = (const ShaderComponent&) = delete;
	ShaderComponent& operator = (ShaderComponent&&) = delete;

private:
	std::string vsFilename;
	std::string fsFilename;
	unsigned int shaderID;
	unsigned int vertShaderID;
	unsigned int fragShaderID;
	std::unordered_map<std::string, unsigned int> uniformMap;
	
public:
	ShaderComponent(Component* parent_, std::string vsFilename_, std::string fsFilename_);
	~ShaderComponent();
	inline unsigned int GetProgram() const { return shaderID;}
	unsigned int GetUniformID(std::string name);
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime ) override;
	void Render() const override;
};


