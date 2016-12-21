#pragma once
#include <memory>

#include "Geometry.h"
#include "Material.h"
#include "Object3D.h"
#include "Math.h"

enum MeshTypes {
	MESH_BASIC = 0,
	MESH_TILE = 1
};

class Mesh : public Object3D
{
public:
	std::shared_ptr<Geometry> geometry;
	std::shared_ptr<Material> material;

	std::vector<Texture> textures = std::vector<Texture>();
	std::vector<Mesh> children = std::vector<Mesh>();

	bool noDraw;
	bool needsUpdate;
	int uid;
	int type;

	Mesh(std::shared_ptr<Geometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr) : 
		geometry(geometry), material(material), uid(Math::uid()) {
		type = MESH_BASIC;
		needsUpdate = true;
		noDraw = false;
	}
};

