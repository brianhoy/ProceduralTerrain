#pragma once
#include "Mesh.h"

namespace Edge {
	enum Edge {
		NONE = 0,
		TOP = 1,
		LEFT = 2,
		BOTTOM = 4,
		RIGHT = 8
	};
}

struct MeshSpecificTileUniforms {
	int edgeMorph;
	GLfloat scale;
	glm::vec2 tileOffset;

	MeshSpecificTileUniforms() {};
	MeshSpecificTileUniforms(int edgeMorph, GLfloat scale, glm::vec2 tileOffset) :
		edgeMorph(edgeMorph), scale(scale), tileOffset(tileOffset) {}
};

class TileMesh : public Mesh {
public:
	MeshSpecificTileUniforms uniforms;
	bool needsUpdate = true;
};