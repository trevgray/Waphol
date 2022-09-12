#include "ShaderComponent.h"
#include "Debug.h"
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
	if (isCreated) return isCreated;
	isCreated = CompileAttach();
	if (isCreated == false) return isCreated;
	isCreated = Link();
	if (isCreated == false) return isCreated;

	SetUniformLocations();
	isCreated = true;
	return isCreated;
}

void ShaderComponent::OnDestroy() {
	glDetachShader(shaderID, fragShaderID);
	glDetachShader(shaderID, vertShaderID);  
	glDeleteShader(fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteProgram(shaderID);
	isCreated = false;
}



bool ShaderComponent::CompileAttach(){
    GLint status;
	try {
		const char* vsText = ReadTextFile(vsFilename.c_str()); //reading from a file - vert
		const char* fsText = ReadTextFile(fsFilename.c_str()); //reading from a file - frag
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
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = vsFilename;
				errorLog +="\nVERT:\n";
				titleLength = errorLog.length();
				glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &errorLogSize); //get the error log length
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(vertShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}

		glCompileShader(fragShaderID); //go compile the shader
		/// Check for errors
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &status); //get the status of the compiler
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = fsFilename;
				errorLog += "\nFRAG:\n";
				titleLength = errorLog.length();
				glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(fragShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}
    
		shaderID = glCreateProgram(); //creating the master shader
		glAttachShader(shaderID, fragShaderID); //attach the vert & frag
		glAttachShader(shaderID, vertShaderID);
		if(vsText) delete[] vsText; //delete memory
		if(fsText) delete[] fsText;

	}catch(std::string error){
		printf("%s\n",&error[0]);
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}


bool ShaderComponent::Link(){
	GLint status;
	try{
			glLinkProgram(shaderID); //linking the vert to the frag - to pass variables between them
			/// Check for errors
			glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				std::string errorLog;
				glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize(errorLogSize);
				glGetProgramInfoLog(shaderID, errorLogSize, &errorLogSize, &errorLog[0]);
				throw errorLog;
			}
		
	}catch(std::string error){
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}

GLuint ShaderComponent::GetUniformID(std::string name) { 	
	auto id = uniformMap.find(name);
#ifdef _DEBUG
	if (id == uniformMap.end()) {
		///Debug::Error(name + " :No such uniform name", __FILE__, __LINE__);
		return -1;
	}
#endif 
	return id->second;
}

void ShaderComponent::SetUniformLocations(){ //synchronize GPU with CPU to optain the active uniforms
	int count;
	GLsizei actualLen;
	GLint size;
	GLenum type;
	char *name;
	int maxUniformListLength;
	unsigned int loc;
	char buffer[512];

	std::string activeUniformList = "Uniforms:\n";
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &count);
	sprintf_s(buffer,512,"There are %d active Uniforms\n",count);

	/// get the length of the longest named uniform 
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength);

	/// Create a little buffer to hold the uniform's name - old C memory call just for fun 
	name = (char *)malloc(sizeof(char) * maxUniformListLength);

	
	for (int i = 0; i < count; ++i) {
		
		/// Get the name of the ith uniform
		
		glGetActiveUniform(shaderID, i, maxUniformListLength, &actualLen, &size, &type, name);
		/// Get the (unsigned int) loc for this uniform
		loc = glGetUniformLocation(shaderID, name);
		std::string uniformName = name;
		uniformMap[uniformName] = loc;

		sprintf_s(buffer,512,"\"%s\" loc:%d\n", uniformName.c_str() , uniformMap[uniformName]);
		activeUniformList += buffer;
	}
	Debug::Info(activeUniformList, __FILE__, __LINE__);
	free(name);
}

/// Read from a specified file and return a char array from the heap 
/// The memory must be deleted within this class. It may not be the best way 
/// to do this but it is all private and I did delete it! SSF
char* ShaderComponent::ReadTextFile(const char *filename){ //open text file and read it
	char* buffer = nullptr;
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {								/// Open the file
        file.seekg(0, std::ios::end);					/// goto the end of the file
        int bufferSize = (int) file.tellg();			/// Get the length of the file - determine buffer size
		if(bufferSize == 0) {							/// If zero, bad file
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
    } else { 
		std::string errorMsg  ("Can't open shader file: ");
		errorMsg += filename;
		Debug::Error(errorMsg, __FILE__, __LINE__);
	}
	return buffer;
}

/// Not used, but must be made concrete
void ShaderComponent::Update(const float deltaTime) {}
void ShaderComponent::Render() const {}