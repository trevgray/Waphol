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
#include <SDL_image.h>

OpenGLRenderer::OpenGLRenderer(){
	setRendererType(RendererType::OPENGL);
}


OpenGLRenderer::~OpenGLRenderer(){
	//if (window) delete window;
	OnDestroy();
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
	glDeleteBuffers(numberOfBuffers, &bufferID); //protects the memory and delete the buffer
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

void OpenGLRenderer::LoadTexture2D(std::string texturePath, unsigned int& textureID) {
	glGenTextures(1, &textureID); //generate name id for each texture
	glBindTexture(GL_TEXTURE_2D, textureID); //connect textureID to be a GL_TEXTURE_2D
	SDL_Surface* textureSurface = IMG_Load(texturePath.c_str()); //load the texture using SDL
	if (textureSurface == nullptr) {
		return;
	}
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB; //are the bytes = 4 or 3 (transparent or not)
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels); //specify a two-dimensional texture image in opengl

	SDL_FreeSurface(textureSurface); //unload texture
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
}

void OpenGLRenderer::RemoveTexture2D(unsigned int& textureID) {
	glDeleteTextures(1, &textureID); //delete the texture
}

void OpenGLRenderer::CreateShader(std::string vertPath, std::string fragPath, unsigned int& shaderID, unsigned int& vertShaderID, unsigned int& fragShaderID, std::unordered_map<std::string, unsigned int>& uniformMap) {
	CompileAttach(vertPath, fragPath, shaderID, vertShaderID, fragShaderID);
	Link(shaderID);
	SetUniformLocations(shaderID, uniformMap);
}

void OpenGLRenderer::DeleteShader(unsigned int& shaderID, unsigned int& vertShaderID, unsigned int& fragShaderID) {
	glDetachShader(shaderID, fragShaderID);
	glDetachShader(shaderID, vertShaderID);
	glDeleteShader(fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteProgram(shaderID);
}

void OpenGLRenderer::ClearScreen(float red, float green, float blue, float alpha) {
	glEnable(GL_DEPTH_TEST);
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::BindBuffer(int bufferType, unsigned int bufferID) {
	glBindBuffer(bufferType, bufferID);
}

void OpenGLRenderer::BindTexture(int textureType, unsigned int textureID) {
	glBindTexture(textureType, textureID);
}

void OpenGLRenderer::UpdateMatrixUniform(unsigned int uniformID, MATH::Matrix4 object) {
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, object);
}

void OpenGLRenderer::UseShader(unsigned int shaderID) {
	glUseProgram(shaderID);
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

bool OpenGLRenderer::CompileAttach(std::string vertPath, std::string fragPath, unsigned int& shaderID, unsigned int& vertShaderID, unsigned int& fragShaderID) {
	GLint status;
	try {
		const char* vsText = ReadTextFile(vertPath.c_str()); //reading from a file - vert
		const char* fsText = ReadTextFile(fragPath.c_str()); //reading from a file - frag
		if (vsText == nullptr || fsText == nullptr) {
			return false;
		}
		/// GL_VERTEX_SHADER and GL_FRAGMENT_SHADER are defined in glew.h
		vertShaderID = glCreateShader(GL_VERTEX_SHADER); //Create name of the shader
		fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		/// Check for errors
		if (vertShaderID == 0 || fragShaderID == 0) { //frag & vert should be > 0
			std::string errorMsg("Can't create a new shader");
			throw errorMsg;
		}

		glShaderSource(vertShaderID, 1, &vsText, 0); //opening port to the gpu - sending the address of the pointer to the gpu - reading the entire shader
		glShaderSource(fragShaderID, 1, &fsText, 0);

		glCompileShader(vertShaderID); //go compile the shader
		/// Check for errors
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &status); //get the status of the compiler
		if (status == 0) {
			GLsizei errorLogSize = 0;
			GLsizei titleLength;
			std::string errorLog = vertPath;
			errorLog += "\nVERT:\n";
			titleLength = errorLog.length();
			glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &errorLogSize); //get the error log length
			errorLog.resize(titleLength + errorLogSize);
			glGetShaderInfoLog(vertShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
			throw errorLog;
		}

		glCompileShader(fragShaderID); //go compile the shader
		/// Check for errors
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &status); //get the status of the compiler
		if (status == 0) {
			GLsizei errorLogSize = 0;
			GLsizei titleLength;
			std::string errorLog = fragPath;
			errorLog += "\nFRAG:\n";
			titleLength = errorLog.length();
			glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
			errorLog.resize(titleLength + errorLogSize);
			glGetShaderInfoLog(fragShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
			throw errorLog;
		}

		shaderID = glCreateProgram(); //creating the master shader
		glAttachShader(shaderID, fragShaderID); //attach the vert & frag
		glAttachShader(shaderID, vertShaderID);
		if (vsText) delete[] vsText; //delete memory
		if (fsText) delete[] fsText;

	}
	catch (std::string error) {
		printf("%s\n", &error[0]);
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}


bool OpenGLRenderer::Link(unsigned int& shaderID) {
	GLint status;
	try {
		glLinkProgram(shaderID); //linking the vert to the frag - to pass variables between them
		/// Check for errors
		glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
		if (status == 0) {
			GLsizei errorLogSize = 0;
			std::string errorLog;
			glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
			errorLog.resize(errorLogSize);
			glGetProgramInfoLog(shaderID, errorLogSize, &errorLogSize, &errorLog[0]);
			throw errorLog;
		}

	}
	catch (std::string error) {
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}

void OpenGLRenderer::SetUniformLocations(unsigned int& shaderID, std::unordered_map<std::string, unsigned int>& uniformMap) { //synchronize GPU with CPU to optain the active uniforms
	int count;
	GLsizei actualLen;
	GLint size;
	GLenum type;
	char* name;
	int maxUniformListLength;
	unsigned int loc;
	char buffer[512];

	std::string activeUniformList = "Uniforms:\n";
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &count);
	sprintf_s(buffer, 512, "There are %d active Uniforms\n", count);

	/// get the length of the longest named uniform 
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength);

	/// Create a little buffer to hold the uniform's name - old C memory call just for fun 
	name = (char*)malloc(sizeof(char) * maxUniformListLength);


	for (int i = 0; i < count; ++i) {

		/// Get the name of the ith uniform

		glGetActiveUniform(shaderID, i, maxUniformListLength, &actualLen, &size, &type, name);
		/// Get the (unsigned int) loc for this uniform
		loc = glGetUniformLocation(shaderID, name);
		std::string uniformName = name;
		uniformMap[uniformName] = loc;

		sprintf_s(buffer, 512, "\"%s\" loc:%d\n", uniformName.c_str(), uniformMap[uniformName]);
		activeUniformList += buffer;
	}
	Debug::Info(activeUniformList, __FILE__, __LINE__);
	free(name);
}

/// Read from a specified file and return a char array from the heap 
/// The memory must be deleted within this class. It may not be the best way 
/// to do this but it is all private and I did delete it! SSF
char* OpenGLRenderer::ReadTextFile(const char* filename) { //open text file and read it
	char* buffer = nullptr;
	std::ifstream file(filename, std::ios::binary);
	if (file.is_open()) {								/// Open the file
		file.seekg(0, std::ios::end);					/// goto the end of the file
		int bufferSize = (int)file.tellg();			/// Get the length of the file - determine buffer size
		if (bufferSize == 0) {							/// If zero, bad file
			std::string errorMsg("Can't read shader file: ");
			std::string str2(filename);
			errorMsg += str2;
			throw errorMsg;								/// Bail out
		}
		buffer = new char[(int)(bufferSize + 1)];		/// Need to NULL terminate the array - making a char array or a "string" - allocating memory array
		file.seekg(0, std::ios::beg);
		file.read(buffer, bufferSize);
		buffer[bufferSize] = NULL;						/// Add the NULL to the end so we know the end of the array
		file.close();
	}
	else {
		std::string errorMsg("Can't open shader file: ");
		errorMsg += filename;
		Debug::Error(errorMsg, __FILE__, __LINE__);
	}
	return buffer;
}