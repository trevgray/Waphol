#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H
#include "Renderer.h"
class OpenGLRenderer: public Renderer {
public:
	OpenGLRenderer();
	~OpenGLRenderer();
	virtual SDL_Window* CreateSDLWindow(std::string name_, int width_, int height_);
	virtual void CreateUniformBuffer(unsigned int& bufferID, unsigned int bindingPoint, std::size_t bufferSize);
	void UpdateUniformBuffer(unsigned int& bufferID, unsigned int offset, size_t bufferSize, void* object);
	void DeleteBuffers(int numberOfBuffers, unsigned int& bufferID);
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
