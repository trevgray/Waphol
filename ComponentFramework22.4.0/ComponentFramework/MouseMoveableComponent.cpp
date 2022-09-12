//#include <iostream>
#include "MouseMoveableComponent.h"
#include "QMath.h"
#include "MMath.h"
#include <glew.h>
using namespace MATH;
MouseMoveableComponent::MouseMoveableComponent(Component* parent_):Component(parent_) {
	setWindowDimensions();
}


MouseMoveableComponent::~MouseMoveableComponent() {
	OnDestroy();
}

bool MouseMoveableComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = true;
	return isCreated;
}
void MouseMoveableComponent::OnDestroy() {
	isCreated = false;
}

void MouseMoveableComponent::Update(const float deltaTime) {

}
void MouseMoveableComponent::Render()const {}

void MouseMoveableComponent::setWindowDimensions() {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	invNDC = MMath::inverse(MMath::viewportNDC(viewport[2], viewport[3]));
}

Vec3 MouseMoveableComponent::getMouseVector(int x, int y, Actor* actor, Matrix4 projectionMatrix) {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	Vec3 mousePosition(static_cast<float>(x), static_cast<float>(y), 0.0f);
	Vec3 v = invNDC * mousePosition; //i've converted to NDC coords, i need to convert out

	Vec4 projPos = MMath::inverse(projectionMatrix) * Vec4(v, 1.0);
	projPos = projPos / projPos.w;
	std::cout << projPos.x << " " << projPos.y << " " << projPos.z << std::endl;
	return projPos;
	//v.x = viewport[2] * (v.x + 1) / 2;

	//Vec3 objectPos = actor->GetComponent<TransformComponent>()->GetPosition();
//Matrix4 invProjectionMatrix = MMath::inverse(projectionMatrix);
//objectPos = invNDC * objectPos;
//std::cout << objectPos.x << " " << objectPos.y << " " << objectPos.z << std::endl;

	/*Vec3 objectPos = actor->GetComponent<TransformComponent>()->GetPosition();
Matrix4 modelMatrix = actor->GetModelMatrix();
double modelview[16];
glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
double projectionMatrix[16];
glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
int viewport[4];
glGetIntegerv(GL_VIEWPORT, viewport);
double* winX = 0;
double* winY = 0;
double* winZ = 0;
gluProject(objectPos.x, objectPos.y, objectPos.z, modelview, projectionMatrix, viewport, winX, winY, winZ);*/
}

void MouseMoveableComponent::SetMousePos2(int x, int y) {
	//lastMousePos = getMouseVector(x, y);
}