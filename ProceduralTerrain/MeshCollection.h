#pragma once
#include <vector>

#include "Mesh.h"
#include "Object3D.h"
#include "Math.h"
class MeshCollection : public Object3D {
public:
	std::vector<std::shared_ptr<Mesh>> meshes = std::vector<std::shared_ptr<Mesh>>();
	std::vector<std::shared_ptr<MeshCollection>> meshCollections = std::vector<std::shared_ptr<MeshCollection>>();
	int uid;

	MeshCollection() : uid(Math::uid()) {}
};