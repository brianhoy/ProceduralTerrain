#pragma once
#include "InputReceiverInterface.h"
#include <GLFW/glfw3.h>
#include <iostream>

class InputManager
{
private:
	GLFWwindow* window;
public:
	InputManager(GLFWwindow* window) : window(window) { }
	void setReceiver(InputReceiverInterface* receiver) {
		this->receiver = receiver;

		glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
		glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mode) {
			reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(w))->key_callback(w, key, scancode, action, mode);
		});
		glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
			reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(w))->mouse_callback(w, xpos, ypos);
		});
		glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
			reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(w))->scroll_callback(w, xoffset, yoffset);
		});
	}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		std::cout << "Pressed" << key << std::endl;
		receiver->key_callback(window, key, scancode, action, mode);
	}
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		receiver->mouse_callback(window, xpos, ypos);
	}
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		receiver->scroll_callback(window, xoffset, yoffset);
	} 
private:
	InputReceiverInterface* receiver;
};

