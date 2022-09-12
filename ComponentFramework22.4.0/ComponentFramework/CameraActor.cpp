#include <glew.h>
#include "CameraActor.h"
#include "TransformComponent.h"
#include "MMath.h"
#include "QMath.h"
#include "Debug.h"
#include "UBO_Padding.h"

CameraActor::CameraActor(Component* parent_):Actor(parent_) {
	uboMatricesID = NULL;
}

CameraActor::~CameraActor() {
	OnDestroy();
}

bool CameraActor::OnCreate() {
	if (isCreated) return isCreated; //return true
	Debug::Info("Creating values for CameraActor: ", __FILE__, __LINE__);
	//buffer
	size_t bufferSize = 2 * UBO_PADDING::MAT4; //*2 because we want both projectionMatrix & viewMatrix
	glGenBuffers(1, &uboMatricesID); //generate a buffer
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID); //I'm talking to you uboMatricesID
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW); //generate the size, allocate memory - GL_STATIC_DRAW is one way to the gpu
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //I'm no longer talking to uboMatricesID - every ubo has a unique ID, 0 is nothing
	//binding - https://www.khronos.org/opengl/wiki/Uniform_Buffer_Object
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboMatricesID); //giving it a binding point, so it can be found later by gpu

	UpdateProjectionMatrix(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	UpdateViewMatrix();
	isCreated = true;
	return isCreated;
}

void CameraActor::UpdateProjectionMatrix(const float fovy, const float aspectRatio, const float near, const float far) {
	projectionMatrix = MMath::perspective(fovy, aspectRatio, near, far);
	//buffer update
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID); //I'm talking to you uboMatricesID
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), projectionMatrix); //offset = 0 because it is the start of the buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //unbind buffer
}

void CameraActor::UpdateViewMatrix() {
	Ref<TransformComponent> transformComponent = GetComponent<TransformComponent>();
	if (transformComponent == nullptr) {
		viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		Quaternion orientation = transformComponent->GetQuaternion();
		Vec3 position = transformComponent->GetPosition();
		viewMatrix = MMath::translate(position) * QMath::toMatrix4(orientation);
	}
	//buffer update
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID); //I'm talking to you uboMatricesID
	glBufferSubData(GL_UNIFORM_BUFFER, UBO_PADDING::MAT4, sizeof(Matrix4), viewMatrix); //offset = sizeof(Matrix4) because we put the projection in before
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //unbind buffer
}

void CameraActor::OnDestroy() {
	glDeleteBuffers(1, &uboMatricesID); //protect the memory and delete the buffer
	isCreated = false;
}