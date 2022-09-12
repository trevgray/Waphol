#pragma once
#include "Component.h"
#include <glew.h>
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
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvCoords;
	size_t dateLength;
	GLenum drawmode;

	/// Private helper methods
	void LoadModel(const char* filename);
	void StoreMeshData(GLenum drawmode_);
	GLuint vao, vbo;
public:

	MeshComponent(Component* parent_, std::string filename_);
	~MeshComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime);
	void Render() const;
	void Render(GLenum drawmode) const;

};