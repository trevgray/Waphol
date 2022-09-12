#pragma once
#include "Component.h"
#include "glew.h"
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
	std::unordered_map<std::string, GLuint > uniformMap;

	/// Private helper methods
	char* ReadTextFile(const char *fileName);	
	bool CompileAttach();
	bool Link();
	void SetUniformLocations();
	

public:
	ShaderComponent(Component* parent_, std::string vsFilename_, std::string fsFilename_);
	~ShaderComponent();
	inline GLuint GetProgram() const { return shaderID;}
	GLuint GetUniformID(std::string name);
	bool OnCreate() override;
	void OnDestroy() override;
	void Update(const float deltaTime ) override;
	void Render() const override;
};


