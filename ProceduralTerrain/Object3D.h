#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object3D {
protected:
	glm::vec3 position;
	glm::quat quaternion;
	glm::vec3 scale;

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
			modelMatrix =  rotationMatrix * translationMatrix * parent->getModelMatrix() * scaleMatrix;
		}
		else {
			modelMatrix = rotationMatrix * translationMatrix * scaleMatrix;
		}
	}
	void updateRotationMatrix() {
		rotationMatrix = glm::mat4(quaternion);
	}

public:
	Object3D* parent = nullptr;
	int version = 0;

	int lastVersion = -1;
	int lastParentVersion = -1;
	int lastVersionUploaded = -1;

	int modelLocation;

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
		return glm::normalize(glm::vec3(modelMatrix[0].z, modelMatrix[1].z, modelMatrix[2].z));
	}
	glm::vec3 getRight() {
		//return glm::normalize(glm::cross(, cameraDirection));
		return glm::normalize(glm::vec3(modelMatrix[0].x, modelMatrix[1].x, modelMatrix[2].x));
	}	
	glm::vec3 getUp() {
		return glm::normalize(glm::vec3(modelMatrix[0].y, modelMatrix[1].y, modelMatrix[2].y));
	}
	glm::mat4 getModelMatrix() {
		if (version != lastVersion || (parent != nullptr && lastParentVersion != parent->version)) {
			if (parent != nullptr) {
				lastParentVersion = parent->version;
			}
			updateMatrix();
		}
		

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

	void add(Object3D* obj) {
		if (obj == this) return;
		obj->parent = this;
	}
	void rotate(float angle, glm::vec3& axis) {
		version++;
		quaternion *= glm::angleAxis(angle, axis);
		updateRotationMatrix();
		updateMatrix();
	}
	void rotate(glm::vec3& eulerAngles) {
		version++;
		quaternion = glm::angleAxis(glm::radians(eulerAngles.x), glm::vec3(1.0f, 0.0f, 0.0f)) * quaternion;
		quaternion = glm::angleAxis(glm::radians(eulerAngles.y), glm::vec3(0.0f, 1.0f, 0.0f)) * quaternion;
		quaternion = glm::angleAxis(glm::radians(eulerAngles.z), glm::vec3(0.0f, 0.0f, 1.0f)) * quaternion;

		updateRotationMatrix();
		updateMatrix();
	}
	void setAngles(glm::vec3& eulerAngles) {
		version++;
		quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		rotate(eulerAngles);

		updateRotationMatrix();
		updateMatrix();
	}
	void setAngles(glm::quat quaternion_) {
		version++;
		quaternion = quaternion_;

		updateRotationMatrix();
		updateMatrix();
	}
	void setScale(glm::vec3 scale_) {
		version++;
		scale = scale_;
		scaleMatrix = glm::scale(glm::mat4(), scale);

		updateMatrix();
	}
	void translate(glm::vec3& translation) {
		version++;
		std::cout << "translation: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;

		position += translation;
		//translationMatrix = glm::translate(translationMatrix, translation);
		setPosition(position);

		updateMatrix();
	}
	void setPosition(glm::vec3 position_) {
		version++;
		position = position_;
		translationMatrix = glm::translate(glm::mat4(), position);

		updateMatrix();
	}
	Object3D* getParent() {
		return parent;
	}
};