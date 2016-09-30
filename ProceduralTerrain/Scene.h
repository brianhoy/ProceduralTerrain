#pragma once

#include "Mesh.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	~Scene();

	void add(Mesh& mesh);
};