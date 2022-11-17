#include "OpenGLRenderer.h"
#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include "Debug.h"

#include <array>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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

void OpenGLRenderer::DeleteVertexArrays(int numberOfBuffers, unsigned int& bufferID) {
	glDeleteVertexArrays(numberOfBuffers, &bufferID);
}

void OpenGLRenderer::LoadModel(std::string modelPath, unsigned int& vao, unsigned int& vbo, size_t& dateLength) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	std::vector<MATH::Vec3> vertices;
	std::vector<MATH::Vec3> normals;
	std::vector<MATH::Vec2> uvCoords;
	vertices.clear();
	normals.clear();
	uvCoords.clear();

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
		throw std::runtime_error(warn + err);
	}
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			MATH::Vec3 vertex{};
			vertex.x = attrib.vertices[3 * index.vertex_index + 0];
			vertex.y = attrib.vertices[3 * index.vertex_index + 1];
			vertex.z = attrib.vertices[3 * index.vertex_index + 2];

			MATH::Vec3 normal{};
			normal.x = attrib.normals[3 * index.normal_index + 0];
			normal.y = attrib.normals[3 * index.normal_index + 1];
			normal.z = attrib.normals[3 * index.normal_index + 2];

			MATH::Vec2 uvCoord{};
			uvCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
			uvCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];

			vertices.push_back(vertex);
			normals.push_back(normal);
			uvCoords.push_back(uvCoord);
		}
	}

	StoreMeshData(vao, vbo, dateLength, vertices, normals, uvCoords);
}

void OpenGLRenderer::RenderModel(unsigned int vao, size_t dateLength, unsigned int drawmode) {
	glBindVertexArray(vao);
	glDrawArrays(drawmode, 0, dateLength);
	glBindVertexArray(0); // Unbind the VAO
}

void OpenGLRenderer::StoreMeshData(unsigned int& vao, unsigned int& vbo, size_t& dateLength,
	std::vector<MATH::Vec3> vertices, std::vector<MATH::Vec3> normals, std::vector<MATH::Vec2> uvCoords) { //made a vertex buffer for opengl

	/// These just make the code easier for me to read
#define VERTEX_LENGTH 	(vertices.size() * (sizeof(MATH::Vec3)))
#define NORMAL_LENGTH 	(normals.size() * (sizeof(MATH::Vec3)))
#define TEXCOORD_LENGTH (uvCoords.size() * (sizeof(MATH::Vec2)))

	const int verticiesLayoutLocation = 0;
	const int normalsLayoutLocation = 1;
	const int uvCoordsLayoutLocation = 2;

	/// create and bind the VOA
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/// Create and initialize vertex buffer object VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH + TEXCOORD_LENGTH, nullptr, GL_STATIC_DRAW);

	/// assigns the addr of "points" to be the beginning of the array buffer "sizeof(points)" in length
	glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_LENGTH, &vertices[0]);
	/// assigns the addr of "normals" to be "sizeof(points)" offset from the beginning and "sizeof(normals)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH, NORMAL_LENGTH, &normals[0]);
	/// assigns the addr of "texCoords" to be "sizeof(points) + sizeof(normals)" offset from the beginning and "sizeof(texCoords)" in length  
	glBufferSubData(GL_ARRAY_BUFFER, VERTEX_LENGTH + NORMAL_LENGTH, TEXCOORD_LENGTH, &uvCoords[0]);

	glEnableVertexAttribArray(verticiesLayoutLocation);
	glVertexAttribPointer(verticiesLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(normalsLayoutLocation);
	glVertexAttribPointer(normalsLayoutLocation, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH));
	glEnableVertexAttribArray(uvCoordsLayoutLocation);
	glVertexAttribPointer(uvCoordsLayoutLocation, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(VERTEX_LENGTH + NORMAL_LENGTH));

	dateLength = vertices.size();

	/// give back the memory used in these vectors. The data is safely stored in the GPU now
	vertices.clear();
	normals.clear();
	uvCoords.clear();

	/// Don't need these defines sticking around anymore
#undef VERTEX_LENGTH
#undef NORMAL_LENGTH
#undef TEXCOORD_LENGTH
}

uint64_t OpenGLRenderer::LoadTexture2D(std::string texturePath)
{
	return uint64_t();
}

uint64_t OpenGLRenderer::CreateShader(std::string vertPath, std::string fragPath)
{
	return uint64_t();
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


