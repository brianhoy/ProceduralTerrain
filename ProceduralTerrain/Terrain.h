#pragma once
#include <vector>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "PlaneGeometry.h"
#include "MeshCollection.h"
#include "MaterialBasic.h"
#include "TileMesh.h"

#define PI 3.14159265359f

struct TerrainSpecificUniforms { 
	glm::vec2 globalOffset;
	GLint heightData; // points to GL_TEXTUREX, which holds heightmap data
	GLint worldSize;
	GLint tileResolution;
};

class Terrain {
private:
	float worldSize;
	float levels;
	float resolution;
	float seed;

	glm::vec2 offset;

	TerrainSpecificUniforms uniforms;

	std::shared_ptr<MaterialBasic> material;
	std::shared_ptr<PlaneGeometry> tileGeometry;
public:
	Terrain(float worldSize, int levels, int resolution) :
		worldSize(worldSize), levels(levels), resolution(resolution) {

		tileGeometry = std::make_shared<PlaneGeometry>(1.0, 1.0, resolution, resolution);
		material = std::make_shared<MaterialBasic>("terrain.frag", "terrain.vert");
		tiles = std::make_shared<MeshCollection>();

		uniforms = TerrainSpecificUniforms();
		uniforms.globalOffset = glm::vec2(0.0, 0.0);
		uniforms.tileResolution = resolution;
		uniforms.worldSize = worldSize;

		glm::mat4 mat = glm::mat4();
		glm::mat4 rotated = glm::rotate(mat, -PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		tileGeometry->applyMatrix(rotated);

		createTiles();
	}

	void createTiles() {
		float initialScale = worldSize / std::pow(2, levels);;

		createTile(-initialScale, initialScale, initialScale, Edge::NONE);
		createTile(-initialScale, 0, initialScale, Edge::NONE);
		createTile(0, 0, initialScale, Edge::NONE);
		createTile(0, -initialScale, initialScale, Edge::NONE);

		for (float scale = initialScale; scale < worldSize; scale *= 2.0f) {
			createTile(-2 * scale, -2 * scale, scale, Edge::BOTTOM | Edge::LEFT);
			createTile(-2 * scale, -scale, scale, Edge::LEFT);
			createTile(-2 * scale, 0, scale, Edge::LEFT);
			createTile(-2 * scale, scale, scale, Edge::TOP | Edge::LEFT);

			createTile(-scale, -2 * scale, scale, Edge::BOTTOM);
			// 2 tiles 'missing' here are in previous layer
			createTile(-scale, scale, scale, Edge::TOP);

			createTile(0, -2 * scale, scale, Edge::BOTTOM);
			// 2 tiles 'missing' here are in previous layer
			createTile(0, scale, scale, Edge::TOP);

			createTile(scale, -2 * scale, scale, Edge::BOTTOM | Edge::RIGHT);
			createTile(scale, -scale, scale, Edge::RIGHT);
			createTile(scale, 0, scale, Edge::RIGHT);
			createTile(scale, scale, scale, Edge::TOP | Edge::RIGHT);
		}
	}

	void createTile(float x, float z, int scale, int edgeMorph) {
		std::shared_ptr<TileMesh> mesh = std::make_shared<TileMesh>(tileGeometry, material);
		MeshSpecificTileUniforms uniforms = MeshSpecificTileUniforms(
			edgeMorph, scale, glm::vec2(x, z));
		mesh->uniforms = uniforms;
		tiles->meshes.push_back(mesh);
	}
	std::shared_ptr<MeshCollection> tiles;
	bool needsUpdate = true;
};