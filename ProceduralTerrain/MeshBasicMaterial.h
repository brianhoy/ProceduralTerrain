#pragma once
#include "Material.h"
#include "Texture.h"
#include "Shader.h"

class MeshBasicMaterial : public Material {
public:
	GLuint program;
	Texture* texture;
	Shader* shader;

	MeshBasicMaterial(Texture* texture = NULL, Shader* shader = new Shader("meshbasic.vert", "meshbasic.frag")) : texture(texture), shader(shader) {
		program = shader->Program;
		std::cout << "Setting program id to " << program << std::endl;
	}

	virtual void use() {
		shader->Use();
	}
};