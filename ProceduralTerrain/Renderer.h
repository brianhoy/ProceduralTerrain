#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Scene.h"
#include "InputReceiverInterface.h"
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
public:
	Renderer();
	~Renderer();
	
	int screenWidth, screenHeight;

	int createWindow(int width, int height);
	void render(Camera* camera, Scene* scene);
	void terminate();
	GLFWwindow* window;
	bool shouldClose();
private:
	void uploadGeometry(Geometry* g);
};

