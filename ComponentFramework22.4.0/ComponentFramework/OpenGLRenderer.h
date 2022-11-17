#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include "Renderer.h"
#include <vector>
#include "Vector.h"
class OpenGLRenderer: public Renderer {
public:
	OpenGLRenderer();
	~OpenGLRenderer();
	SDL_Window* CreateSDLWindow(std::string name_, int width_, int height_);
	void CreateUniformBuffer(unsigned int& bufferID, unsigned int bindingPoint, std::size_t bufferSize);
	void UpdateUniformBuffer(unsigned int& bufferID, unsigned int offset, size_t bufferSize, void* object);
	void DeleteBuffers(int numberOfBuffers, unsigned int& bufferID);
	void DeleteVertexArrays(int numberOfBuffers, unsigned int& bufferID);

	void LoadModel(std::string modelPath, unsigned int& vao, unsigned int& vbo, size_t& dateLength);
	void RenderModel(unsigned int vao, size_t dateLength, unsigned int drawmode);
	void StoreMeshData(unsigned int& vao, unsigned int& vbo, size_t& dateLength, std::vector<MATH::Vec3> vertices, std::vector<MATH::Vec3> normals, std::vector<MATH::Vec2> uvCoords);
	uint64_t LoadTexture2D(std::string texturePath);
	uint64_t CreateShader(std::string vertPath, std::string fragPath);

	std::array<int, 4> GetViewPort();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Render();

	//
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	SDL_Window* getWindow() const { return window; }

	SDL_GLContext GetContext() const { return context; }
private:
	int width, height;
	SDL_Window* window;
	SDL_GLContext context;
	void setAttributes(int major_, int minor_);
	void getInstalledOpenGLInfo(int* major, int* minor);
};

#endif
