#pragma once
#include "Component.h"
#include <iostream>
class MaterialComponent: public Component {
	MaterialComponent(const MaterialComponent&) = delete;
	MaterialComponent(MaterialComponent&&) = delete;
	MaterialComponent& operator = (const MaterialComponent&) = delete;
	MaterialComponent& operator = (MaterialComponent&&) = delete;
public:
	MaterialComponent(Component* parent_, std::string filename_);
	virtual ~MaterialComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	inline unsigned int getTextureID() const { return textureID; }

	bool SetNewTexture(std::string filename);
private:
	unsigned int textureID;
	std::string filename;
};