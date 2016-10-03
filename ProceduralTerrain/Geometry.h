#pragma once
#include <vector>
#include "Vertex.h"

class Geometry {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;
};