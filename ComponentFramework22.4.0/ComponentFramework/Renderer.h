#ifndef RENDERER_H
#define RENDERER_H
#include <SDL.h>
#include <string>
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
	virtual void Render() = 0;
	RendererType getRendererType() { return rendererType; }
	void setRendererType(RendererType rendererType_) { rendererType = rendererType_; }
	//////////
	virtual void LoadModel(std::string modelPath, unsigned int& vao, unsigned int& vbo, size_t& dateLength) = 0;
	virtual void RenderModel(unsigned int vao, size_t dateLength, unsigned int drawmode) = 0;
	virtual uint64_t LoadTexture2D(std::string texturePath) = 0;
	virtual uint64_t CreateShader(std::string vertPath, std::string fragPath) = 0; /* , std::string tesPath, std::string tesControlPath, std::string geometryPath) = 0;*/

	virtual void CreateUniformBuffer(unsigned int& bufferID, unsigned int bindingPoint, std::size_t bufferSize) = 0;
	virtual void UpdateUniformBuffer(unsigned int& bufferID, unsigned int offset, size_t bufferSize, void* object) = 0;
	virtual void DeleteBuffers(int numberOfBuffers, unsigned int& bufferID) = 0;
	virtual void DeleteVertexArrays(int numberOfBuffers, unsigned int& bufferID) = 0;

	virtual std::array<int,4> GetViewPort() = 0;

protected:
	Renderer *renderer;
	RendererType rendererType;
};

#endif