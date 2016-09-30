#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object3D {
public:
	glm::vec3 position;
	glm::quat quaternion;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 matrix;

	Object3D() {
		quaternion = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void rotate(double x, double y, double z) {
		rotation.x = x;
		rotation.y = y;
		rotation.z = z;
		matrix = glm::rotate(matrix, 90.0f, rotation);
	}

	void updateMatrix() {
		matrix = glm::translate(matrix, position);
		matrix = glm::rotate(matrix, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, rotation.y, glm::vec3(1.0f, 0))
		
	}
};