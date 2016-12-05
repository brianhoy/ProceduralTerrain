#pragma once
#include <vector>
#include <string>
#include "Texture.h"

class Material {
public:
	GLuint program;
	virtual std::string getType() = 0;

	bool needsUpdate = true;
	GLuint uboPerMaterial;
};
