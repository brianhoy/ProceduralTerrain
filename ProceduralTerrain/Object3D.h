#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object3D {
protected:
	glm::vec3 position;
	glm::quat quaternion;
	glm::vec3 scale;

	Object3D* parent = nullptr;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 modelMatrix;

	glm::vec3 multiplyVector(glm::vec3 vec) {
		glm::vec4 calc = modelMatrix * glm::vec4(vec, 1.0f);
		return glm::vec3(calc.x, calc.y, calc.z);
	}
	void updateMatrix() {
		if (parent != nullptr) {
			modelMatrix = parent->getModelMatrix() * translationMatrix * rotationMatrix * scaleMatrix;
		}
		else {
			modelMatrix = rotationMatrix * translationMatrix * scaleMatrix;
		}
	}
	void updateRotationMatrix() {
		rotationMatrix = glm::mat4(quaternion);
	}

public:
	Object3D() {
		quaternion = glm::quat();
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		scale = glm::vec3(0.0f, 0.0f, 0.0f);

		translationMatrix = glm::mat4();
		rotationMatrix = glm::mat4(quaternion);
		scaleMatrix = glm::mat4();

		updateMatrix();
	}

	glm::vec3 getFront() {
		return multiplyVector(glm::vec3(0.0f, 0.0f, -1.0f));
	}
	glm::vec3 getRight() {
		return multiplyVector(glm::vec3(1.0f, 0.0f, 0.0f));
	}	
	glm::vec3 getUp() {
		return multiplyVector(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	glm::mat4 getModelMatrix() {
		return modelMatrix;
	}
	glm::vec3 getPosition() {
		if (parent == nullptr) return position;
		return position + parent->getPosition();
	}
	glm::quat getQuaternion() {
		if (parent == nullptr) return quaternion;
		return quaternion + parent->getQuaternion();
	}

	glm::vec3 getEulerAngles() {
		glm::vec3 euler = glm::eulerAngles(getQuaternion());
		return glm::vec3(glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z));
	}

	void add(Object3D& obj) {
		if (&obj == this) return;
		obj.parent = this;
	}
	void rotate(float angle, glm::vec3& axis) {
		quaternion *= glm::angleAxis(angle, axis);
		updateRotationMatrix();
		updateMatrix();
	}
	void rotate(glm::vec3& eulerAngles) {
		quaternion = glm::angleAxis(glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f)) * quaternion;
		quaternion = glm::angleAxis(glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f)) * quaternion;
		quaternion = glm::angleAxis(glm::radians(eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f)) * quaternion;

		updateRotationMatrix();
		updateMatrix();
	}
	void setAngles(glm::vec3& eulerAngles) {
		quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		rotate(eulerAngles);

		updateRotationMatrix();
		updateMatrix();
	}
	void setAngles(glm::quat quaternion_) {
		quaternion = quaternion_;

		updateRotationMatrix();
		updateMatrix();
	}
	void setScale(glm::vec3 scale_) {
		scale = scale_;
		scaleMatrix = glm::scale(glm::mat4(), scale);

		updateMatrix();
	}
	void translate(glm::vec3& translation) {
		std::cout << "translation: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;

		position += translation;
		translationMatrix = glm::translate(translationMatrix, translation);
		//setPosition(position);

		updateMatrix();
	}
	void setPosition(glm::vec3 position_) {
		position = position_;
		translationMatrix = glm::translate(glm::mat4(), position);

		updateMatrix();
	}
};