#pragma once
#include <vector>
#include "Vertex.h"
#include "Renderer.h"

class Geometry {
	friend class Renderer;
public:
	bool needsUpdate;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint VAO;
private:
	GLuint VBO, EBO; // only the VAO needs to be seen by the renderer
};