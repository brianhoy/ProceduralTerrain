#pragma once
#include "Player.h"
#include "Camera.h"
#include "InputReceiverInterface.h"
#include "Directions.h"

#include <iostream>

class LocalPlayer : public Player, public InputReceiverInterface
{
public:
	Camera camera;

	LocalPlayer(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		GLfloat movementSpeed = 10.0, GLfloat mouseSensitivity = 0.005f);
	~LocalPlayer();

	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processKeyboard(GLfloat deltaTime);
	void processMouse(double xoffset, double yoffset);
	void doMovement(Direction direction, GLfloat deltaTime);
	void doCameraRotation(double xdelta, double ydelta);
	void update(GLfloat delta);
private:
	bool keys[1024];
	bool firstMouse;
	double lastX, lastY;
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
};

