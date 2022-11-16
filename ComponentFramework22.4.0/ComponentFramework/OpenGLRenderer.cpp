#include "OpenGLRenderer.h"
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include "Debug.h"

#include <array>

OpenGLRenderer::OpenGLRenderer(){
	setRendererType(RendererType::OPENGL);
}


OpenGLRenderer::~OpenGLRenderer(){
	//if (window) delete window;
}


bool OpenGLRenderer::OnCreate() { 
	context = SDL_GL_CreateContext(window);
	int major, minor;
	getInstalledOpenGLInfo(&major, &minor);
	setAttributes(major, minor);

	/// Fire up the GL Extension Wrangler
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		Debug::FatalError("Glew initialization failed", __FILE__, __LINE__);
		return false;
	}
	glViewport(0, 0, width, height);
	return false;
}


void OpenGLRenderer::OnDestroy() {

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}



void OpenGLRenderer::Render() {
	///SDL_GL_SwapWindow(window->getWindow());
}


SDL_Window* OpenGLRenderer::CreateSDLWindow(std::string name_, int width_, int height_){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		Debug::FatalError("Failed to initialize SDL", __FILE__, __LINE__);
		return nullptr;
	}

	this->width = width_;
	this->height = height_;
	window = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_OPENGL);

	if (window == nullptr) {
		Debug::FatalError("Failed to create a window", __FILE__, __LINE__);
		return nullptr;
	}
	return window;
}

void OpenGLRenderer::CreateUniformBuffer(unsigned int& bufferID, unsigned int bindingPoint, std::size_t bufferSize) {
	glGenBuffers(1, &bufferID); //generate a buffer
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID); //I'm talking to you uboMatricesID
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW); //generate the size, allocate memory - GL_STATIC_DRAW is one way to the gpu
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //I'm no longer talking to uboMatricesID - every ubo has a unique ID, 0 is nothing
	//binding - https://www.khronos.org/opengl/wiki/Uniform_Buffer_Object
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, bufferID); //giving it a binding point, so it can be found later by gpu
}

void OpenGLRenderer::UpdateUniformBuffer(unsigned int& bufferID, unsigned int offset, size_t bufferSize, void* object) {
	glBindBuffer(GL_UNIFORM_BUFFER, bufferID); //I'm talking to you uboMatricesID
	glBufferSubData(GL_UNIFORM_BUFFER, offset, bufferSize, object); //offset = 0 because it is the start of the buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //unbind buffer
}

void OpenGLRenderer::DeleteBuffers(int numberOfBuffers, unsigned int& bufferID) {
	glDeleteBuffers(numberOfBuffers, &bufferID); //protect the memory and delete the buffer
}

std::array<int, 4> OpenGLRenderer::GetViewPort() {
	std::array<int, 4> returnViewPort;
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	for (int x = 0; x < 4; x++) {
		returnViewPort[x] = viewport[x];
	}

	return returnViewPort;
}

void OpenGLRenderer::getInstalledOpenGLInfo(int* major, int* minor) {
	/// You can to get some info regarding versions and manufacturer
	const GLubyte* version = glGetString(GL_VERSION);
	/// You can also get the version as ints	
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);


	glGetIntegerv(GL_MAJOR_VERSION, major);
	glGetIntegerv(GL_MINOR_VERSION, minor);
	Debug::Info("OpenGL version: " + std::string((char*)glGetString(GL_VERSION)), __FILE__, __LINE__);
	Debug::Info("Graphics card vendor " + std::string((char*)vendor), __FILE__, __LINE__);
	Debug::Info("Graphics card name " + std::string((char*)renderer), __FILE__, __LINE__);
	Debug::Info("GLSL Version " + std::string((char*)glslVersion), __FILE__, __LINE__);
	return;
}

void OpenGLRenderer::setAttributes(int major_, int minor_) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major_);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor_);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
	return;
}


