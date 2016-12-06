#pragma once
#include <vector>

#include "Mesh.h"
#include "Object3D.h"
#include "Math.h"
class MeshCollection : public Object3D {
public:
	std::vector<Mesh> meshes = std::vector<Mesh>();
	std::vector<MeshCollection> meshCollections = std::vector<MeshCollection>();
	int uid;

	MeshCollection() : uid(Math::uid()) {}
};