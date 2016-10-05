#pragma once

#include <glm/glm.hpp>

class Player
{
public:
	glm::vec3 position;

	Player(glm::vec3 position);
	~Player();
};

