#pragma once
#include "Actor.h"
#include "Vector.h"
using namespace MATH;

enum class LightStyle { 
	DirectionLight = 1, 
	PointLight,
	SkyLight,
	SpotLight
};
class LightActor : public Actor {
	LightActor(const LightActor&) = delete;
	LightActor(LightActor&&) = delete;
	LightActor& operator = (const LightActor&) = delete;
	LightActor& operator = (LightActor&&) = delete;
private:
	LightStyle lightStyle;
	Vec3 pos;
	Vec4 colour;
	float intensity;
	Vec3 fallOff;
	GLuint uboLightDataID;
	GLuint bindpoint = 1;
public:
	LightActor(Component* parent_, LightStyle lightStyle_, Vec3 location_, Vec4 colour_, float intensity_ = 1.0f, Vec3 fallOff_ = Vec3(0.0f,0.0f,0.0f));
	LightActor(Component* parent_, std::string lightStyle_, Vec3 location_, Vec4 colour_, float intensity_ = 1.0f, Vec3 fallOff_ = Vec3(0.0f, 0.0f, 0.0f));
	LightActor(Component* parent_);
	~LightActor();
	Vec3 getPosition() const { return pos; }
	bool OnCreate() override;
	void OnDestroy() override;
	GLuint GetLightID() const { return uboLightDataID; }
};