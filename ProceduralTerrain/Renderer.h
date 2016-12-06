#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Scene.h"
#include "InputReceiverInterface.h"
#include "MaterialBasicUploader.h"
#include "MeshCollection.h"
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
public:
	Renderer();
	
	int screenWidth, screenHeight;

	int createWindow(int width, int height);
	void render(Camera* camera, Scene* scene);
	void renderMeshRecursive(Camera * camera, Mesh * mesh);
	void terminate();
	GLFWwindow* window;
	bool shouldClose();
private:
	void uploadGeometry(Geometry* geometry);
	void uploadMaterial(Material* material);
	void uploadTexture(Texture* texture);

	GLuint createProgram(std::vector<GLuint> shaders);
	void initializeUniformBuffer();

	void updateProjectionMatrix(glm::mat4 projection);
	void updateViewMatrix(glm::mat4 view);

	GLuint uboMatrices;

	MaterialBasicUploader materialBasicUploader;
};
