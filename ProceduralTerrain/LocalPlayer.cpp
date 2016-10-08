#include "LocalPlayer.h"



LocalPlayer::LocalPlayer(glm::vec3 position, GLfloat movementSpeed = 10.0) :
	Player(position), camera(Camera(glm::vec3(0.0f, 0.0f, 0.0f))), movementSpeed(movementSpeed)
{
}


LocalPlayer::~LocalPlayer()
{
}

void LocalPlayer::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void LocalPlayer::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	processMouse(xoffset, yoffset);
}

void LocalPlayer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void LocalPlayer::processMouse(double xoffset, double yoffset) {
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void LocalPlayer::processKeyboard(GLfloat deltaTime) {
	if (keys[GLFW_KEY_W]) doMovement(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S]) doMovement(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A]) doMovement(LEFT, deltaTime);
	if (keys[GLFW_KEY_D]) doMovement(RIGHT, deltaTime);
}

void LocalPlayer::doMovement(Direction direction, GLfloat deltaTime)
{
	GLfloat velocity = deltaTime * movementSpeed;
	switch (direction) {
	case FORWARD:
		position += getFront() * velocity;
	case BACKWARD:
		position -= getFront() * velocity;
	case LEFT:
		position -= getRight() * velocity;
	case RIGHT:
		position += getRight() * velocity;
	}
}
