#include "MaterialComponent.h"
#include <SDL_image.h>

MaterialComponent::MaterialComponent(Component* parent_, std::string filename_):Component(parent_), filename(filename_), textureID(0) {}

MaterialComponent::~MaterialComponent() {
	OnDestroy();
}

bool MaterialComponent::OnCreate() {
	if (isCreated) return isCreated;
	isCreated = LoadImage(filename);
	return isCreated;
}

void MaterialComponent::OnDestroy() {
	glDeleteTextures(1, &textureID); //delete the texture
	isCreated = false;
}

void MaterialComponent::Update(const float deltaTime) {}

void MaterialComponent::Render()const {}

bool MaterialComponent::LoadImage(std::string filename) {
	glGenTextures(1, &textureID); //generate name id for each texture
	glBindTexture(GL_TEXTURE_2D, textureID); //connect textureID to be a GL_TEXTURE_2D
	SDL_Surface* textureSurface = IMG_Load(filename.c_str()); //load the texture using SDL
	if (textureSurface == nullptr) {
		return false;
	}
	int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB; //are the bytes = 4 or 3 (transparent or not)
	glTexImage2D(GL_TEXTURE_2D, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels); //specify a two-dimensional texture image in opengl

	SDL_FreeSurface(textureSurface); //unload texture
	/// Wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); /// Unbind the texture
	return true;
}
