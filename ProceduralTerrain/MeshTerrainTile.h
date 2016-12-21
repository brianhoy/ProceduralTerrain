#pragma once
#include "Mesh.h"

enum Edge {
	EDGE_NONE = 0,
	EDGE_TOP = 1,
	EDGE_LEFT = 2,
	EDGE_BOTTOM = 4,
	EDGE_RIGHT = 8
};

struct MeshSpecificTileUniforms {
	int edgeMorph;
	GLfloat scale;
	glm::vec2 tileOffset;

	MeshSpecificTileUniforms() {};
	MeshSpecificTileUniforms(int edgeMorph, GLfloat scale, glm::vec2 tileOffset) :
		edgeMorph(edgeMorph), scale(scale), tileOffset(tileOffset) {}
};

class MeshTerrainTile : public Mesh {
public:
	MeshSpecificTileUniforms uniforms;
	MeshTerrainTile(std::shared_ptr<Geometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr) :
		Mesh(geometry, material) {
		type = MESH_TILE;
	}
};