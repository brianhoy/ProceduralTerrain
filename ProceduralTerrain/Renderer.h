#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Scene.h"
#include "Renderer.h"
#include "InputReceiverInterface.h"

class Renderer
{
public:
	Renderer();
	~Renderer();
	
	int createWindow(int width, int height);
	void render(Camera& camera, Scene& scene);
	void terminate();
	GLFWwindow* window;
	bool shouldClose();
};

