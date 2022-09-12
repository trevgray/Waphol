#include <glew.h>
#include "LightActor.h"
#include "UBO_Padding.h" //uniform buffer object padding

LightActor::LightActor(Component* parent_, LightStyle lightStyle_, Vec3 location_, Vec4 colour_, float intensity_, Vec3 fallOff_) : Actor(parent_), 
lightStyle(lightStyle_), pos(location_), intensity(intensity_), colour(colour_), uboLightDataID(NULL) {
}

LightActor::LightActor(Component* parent_, std::string lightStyle_, Vec3 location_, Vec4 colour_, float intensity_, Vec3 fallOff_) : Actor(parent_), pos(location_), intensity(intensity_), colour(colour_), uboLightDataID(NULL) {
	if (lightStyle_ == "PointLight") {
		lightStyle = LightStyle::PointLight;
	}
	else if (lightStyle_ == "SkyLight") {
		lightStyle = LightStyle::SkyLight;
	}
	else if (lightStyle_ == "SpotLight") {
		lightStyle = LightStyle::SpotLight;
	}
	else {
		lightStyle = LightStyle::DirectionLight;
	}
}

LightActor::LightActor(Component* parent_) : Actor(parent_) {
	pos = Vec3(0.0f, 10.0f, 0.0f);
	colour = Vec4(0.8f, 0.8f, 0.8f, 0.0f);
	intensity = 1.0f;
	fallOff = Vec3(0.0f, 0.0f, 0.0f);
}

LightActor::~LightActor() {
	OnDestroy();
}

bool LightActor::OnCreate() {
	if (isCreated) return isCreated;
	size_t bufferSize = UBO_PADDING::VEC3 + UBO_PADDING::VEC4; //could just be UBO_PADDING::VEC4 * 2 - scott likes to readable code

	glGenBuffers(1, &uboLightDataID); //look in cameraActor for all the comments about glBuffers (i'm lazy) - shoot me later
	glBindBuffer(GL_UNIFORM_BUFFER, uboLightDataID);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
	size_t offset = 0; //the artist is what i think everytime - useful comments
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec3), pos);
	offset = UBO_PADDING::VEC3; //not even bindings like before, so have to use the UBO_Padding instead of sizeof()
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(Vec4), colour);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindpoint, uboLightDataID);

	isCreated = true;
	return isCreated;
}

void LightActor::OnDestroy() {
	glDeleteBuffers(1, &uboLightDataID);
	isCreated = false;
}
