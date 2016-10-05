#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object3D {
public:
	glm::vec3 position;
	glm::quat quaternion;
	glm::vec3 eulerRotation; /* Write-only within this class */
	glm::vec3 scale;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	glm::mat4 modelMatrix;

	Object3D() {
		quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		eulerRotation = glm::vec3(0.0f, 0.0f, 0.0f);
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		scale = glm::vec3(0.0f, 0.0f, 0.0f);

		translationMatrix = glm::mat4();
		rotationMatrix = glm::mat4();
		scaleMatrix = glm::mat4();

		updateMatrix();
	}
	void rotate(float angle, glm::vec3& axis) {
		quaternion += glm::angleAxis(angle, axis);
		eulerRotation = glm::eulerAngles(quaternion);

		updateMatrix();
	}
	void rotate(glm::vec3& eulerAngles) {
		quaternion += glm::angleAxis(eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
		quaternion += glm::angleAxis(eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		quaternion += glm::angleAxis(eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));
		rotationMatrix += glm::mat4(quaternion);
		eulerRotation = glm::eulerAngles(quaternion);

		updateMatrix();
	}
	void setAngles(glm::vec3& eulerAngles) {
		quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		rotate(eulerAngles);

		updateMatrix();
	}
	void setAngles(glm::quat quaternion_) {
		quaternion = quaternion_;
		eulerRotation = glm::eulerAngles(quaternion);

		updateMatrix();
	}
	void setScale(glm::vec3 scale_) {
		scale = scale_;
		scaleMatrix = glm::scale(scaleMatrix, scale);

		updateMatrix();
	}
	void translate(glm::vec3& translation) {
		position += translation;
		translationMatrix = glm::translate(translationMatrix, translation);

		updateMatrix();
	}
	void setPosition(glm::vec3 position_) {
		position = position_;
		translationMatrix = glm::translate(glm::mat4(), position);

		updateMatrix();
	}
	void updateMatrix() {
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}
};