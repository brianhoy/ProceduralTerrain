#pragma once
#include "PlaneGeometry.h"
#include "TerrainGenerator.h"
#include "Mesh.h"
#include <vector>

class Terrain {
private:
	float worldWidth;
	float levels;
	float resolution;
	float seed;

	glm::vec2 offset;
	TerrainGenerator terrainGenerator;

	std::vector<Mesh> tiles;
	PlaneGeometry tileGeometry;
public:
	Terrain(int worldWidth, int levels, int resolution) :
		worldWidth(worldWidth), levels(levels), resolution(resolution), 
		tileGeometry(PlaneGeometry(1.0, 1.0, resolution, resolution)) {
		tileGeometry.rotate
	}
};