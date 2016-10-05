#pragma once
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

class MeshBasicMaterial : public Material {
public:
	Texture* texture;
	Shader* shader;

	MeshBasicMaterial(Texture* texture, Shader* shader) : texture(texture), shader(shader) {

	}

	virtual void use() {
		shader->Use();
	}
};