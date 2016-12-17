#pragma once
#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "PlaneGeometry.h"
#include "TerrainGenerator.h"
#include "MeshCollection.h"
#include "MaterialBasic.h"

#define PI 3.14159265359f

class Terrain {
private:
	float worldSize;
	float initialScale;
	float levels;
	float resolution;
	float seed;

	glm::vec2 offset;
	TerrainGenerator terrainGenerator;

	std::shared_ptr<MaterialBasic> material;
	std::shared_ptr<PlaneGeometry> tileGeometry;
public:
	Terrain(float worldSize, int levels, int resolution) :
		worldSize(worldSize), levels(levels), resolution(resolution) {
		initialScale = worldSize / std::pow(2, levels);

		tileGeometry = std::make_shared<PlaneGeometry>(1.0, 1.0, resolution, resolution);
		material = std::make_shared<MaterialBasic>("terrain.frag", "terrain.vert");
		tiles = std::make_shared<MeshCollection>();

		glm::mat4 mat = glm::mat4();
		glm::mat4 rotated = glm::rotate(mat, -PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		tileGeometry->applyMatrix(rotated);

	}

	void createTiles() {

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(tileGeometry, material);

		tiles->meshes.push_back(mesh);
	}

	void createTile(float x, float z, int scale, int edgeMorph) {

	}
	std::shared_ptr<MeshCollection> tiles;
};