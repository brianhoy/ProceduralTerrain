#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "Object3D.h"
#include <vector>

class Scene
{
public:
	Scene() {
		meshes = std::vector<Mesh*>();
		objects = std::vector<Object3D*>();
	}

	std::vector<Object3D*> objects;
	std::vector<Mesh*> meshes;

	void add(Mesh* mesh) {
		meshes.push_back(mesh);
	}

	void add(Object3D* obj) {
		objects.push_back(obj);
	}
};