#ifndef RENDERER_H
#define RENDERER_H
#include <SDL.h>
#include <string>
#include <unordered_map>

#include "Matrix.h" //don't love including it here, but i need it for UpdateMatrixUniform

enum class RendererType {
	NONE,
	OPENGL,
	VULKAN,
	DIRECTX11,
	DIRECTX12
};

class Renderer {
public:
	Renderer():renderer(nullptr),rendererType(RendererType::NONE){}
	virtual ~Renderer() {}
	virtual SDL_Window* CreateSDLWindow(std::string name_, int width_, int height_) = 0;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	RendererType getRendererType() { return rendererType; }
	void setRendererType(RendererType rendererType_) { rendererType = rendererType_; }
	//////////
	virtual void LoadModel(std::string modelPath, unsigned int& vao, unsigned int& vbo, size_t& dateLength) = 0;
	virtual void RenderModel(unsigned int vao, size_t dateLength, unsigned int drawmode) = 0;

	virtual void LoadTexture2D(std::string texturePath, unsigned int& textureID) = 0;
	virtual void RemoveTexture2D(unsigned int& textureID) = 0;

	virtual void CreateShader(std::string vertPath, std::string fragPath, unsigned int& shaderID, unsigned int& vertShaderID, unsigned int& fragShaderID, std::unordered_map<std::string, unsigned int>& uniformMap) = 0; /* , std::string tesPath, std::string tesControlPath, std::string geometryPath) = 0;*/
	virtual void DeleteShader(unsigned int& shaderID, unsigned int& vertShaderID, unsigned int& fragShaderID) = 0;

	virtual void CreateUniformBuffer(unsigned int& bufferID, unsigned int bindingPoint, std::size_t bufferSize) = 0;
	virtual void UpdateUniformBuffer(unsigned int& bufferID, unsigned int offset, size_t bufferSize, void* object) = 0;

	virtual void DeleteBuffers(int numberOfBuffers, unsigned int& bufferID) = 0;
	virtual void DeleteVertexArrays(int numberOfBuffers, unsigned int& bufferID) = 0;

	virtual void ClearScreen(float red, float green, float blue, float alpha) = 0;
	virtual void BindBuffer(int bufferType, unsigned int bufferID) = 0;
	virtual void BindTexture(int textureType, unsigned int textureID) = 0;

	virtual void UpdateMatrixUniform(unsigned int uniformID, MATH::Matrix4 object) = 0;
	//virtual void UpdateUniform(unsigned int uniformID, void* object, maybe templated for the uniform type?);

	virtual void UseShader(unsigned int shaderID) = 0;

	virtual std::array<int,4> GetViewPort() = 0;

protected:
	Renderer *renderer;
	RendererType rendererType;
};

#endif