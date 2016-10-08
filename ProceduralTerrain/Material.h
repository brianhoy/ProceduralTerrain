#pragma once
#include "Renderer.h"
class Material {
public:
	GLuint program;
	virtual void use() = 0;
};