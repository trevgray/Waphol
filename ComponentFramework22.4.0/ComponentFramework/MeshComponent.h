#pragma once
#include "Component.h"
#include <vector>
#include "Vector.h"
using namespace MATH;

class MeshComponent : public Component {
	MeshComponent(const MeshComponent&) = delete;
	MeshComponent(MeshComponent&&) = delete;
	MeshComponent& operator = (const MeshComponent&) = delete;
	MeshComponent& operator = (MeshComponent&&) = delete;

private:
	std::string filename;

	/// Private helper methods
	void LoadModel(const char* filename);
	unsigned int vao, vbo;
	size_t dateLength;
public:

	MeshComponent(Component* parent_, std::string filename_);
	~MeshComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	void Render(unsigned int drawmode) const;

};