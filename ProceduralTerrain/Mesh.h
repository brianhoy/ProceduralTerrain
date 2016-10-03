#pragma once
#include "Geometry.h"
#include "Material.h"
#include "Object3D.h"

class Mesh : public Object3D
{
public:
	Geometry* geometry;
	Material* material;

	Mesh(Geometry* geometry, Material* material) : geometry(geometry), material(material) {
	}
private:
	int drawMode;
};

