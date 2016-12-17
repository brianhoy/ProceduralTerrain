#pragma once
#include "Mesh.h"



class MeshTerrain : public Mesh {
	int edgeMorph;
	int	scale;
	glm::vec2 tileOffset;
	float tileResolution;

};