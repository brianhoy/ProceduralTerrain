#pragma once
#include "Geometry.h"
#include "Material.h"

class Mesh
{
public:
	Geometry* geometry;
	Material* material;

	Mesh(Geometry* geometry, Material* material) : geometry(geometry), material(material) {
	}
private:
	int drawMode;
};

