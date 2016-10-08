#pragma once

#include <glm/glm.hpp>
#include "Object3D.h"

class Player : public Object3D
{
public:
	Player(glm::vec3 position);
	~Player();
};

