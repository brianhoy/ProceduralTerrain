#pragma once
#include <GLFW/glfw3.h>

class InputReceiverInterface
{
public:
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) = 0;
	virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos) = 0;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
};

