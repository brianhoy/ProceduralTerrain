#pragma once
#include "Geometry.h"
#include "Material.h"
#include "Object3D.h"

class Mesh : public Object3D
{
public:
	Geometry* geometry;
	Material* material;

	std::vector<Mesh> children;
	bool noDraw = false;

	Mesh(Geometry* geometry = nullptr, Material* material = nullptr, std::vector<Mesh> children = std::vector<Mesh>()) : 
		geometry(geometry), material(material), children(children) {

	}
private:
	int drawMode;
};

