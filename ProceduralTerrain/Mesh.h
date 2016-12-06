#pragma once
#include "Geometry.h"
#include "Material.h"
#include "Object3D.h"
#include "Math.h"

class Mesh : public Object3D
{
public:
	Geometry* geometry;
	Material* material;

	std::vector<Texture> textures = std::vector<Texture>();
	std::vector<Mesh> children = std::vector<Mesh>();
	bool noDraw = false;
	int uid;

	Mesh(Geometry* geometry = nullptr, Material* material = nullptr) : 
		geometry(geometry), material(material), uid(Math::uid()) {
	}
private:
	int drawMode;
};

