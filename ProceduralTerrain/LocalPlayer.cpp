#include "LocalPlayer.h"



LocalPlayer::LocalPlayer(glm::vec3 position, GLfloat movementSpeed, GLfloat mouseSensitivity) :
	Player(position), camera(Camera(glm::vec3(0.0f, 0.0f, 0.0f))), movementSpeed(movementSpeed), mouseSensitivity(mouseSensitivity)
{
	add(&camera);
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
	GLfloat yoffset = ypos - lastY; 

	lastX = xpos;
	lastY = ypos;

	processMouse(xoffset, yoffset);
}

void LocalPlayer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//camera.ProcessMouseScroll(yoffset);
}

void LocalPlayer::processMouse(double xoffset, double yoffset) {
	doCameraRotation(xoffset, yoffset);
}

void LocalPlayer::processKeyboard(GLfloat deltaTime) {
	if (keys[GLFW_KEY_W]) doMovement(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S]) doMovement(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A]) doMovement(LEFT, deltaTime);
	if (keys[GLFW_KEY_D]) doMovement(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE]) doMovement(UP, deltaTime);
	if (keys[GLFW_KEY_LEFT_SHIFT]) doMovement(DOWN, deltaTime);
}

void LocalPlayer::doMovement(Direction direction, GLfloat deltaTime)
{

	glm::vec3 right = getRight(); //glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 front = getFront(); //glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = getUp();

	GLfloat velocity = deltaTime * movementSpeed;
	if(direction == FORWARD)
		translate(front * velocity);
	if (direction == BACKWARD)
		translate(-1.0f * front * velocity);
	if (direction == LEFT)
		translate(1.0f * right * velocity);
	if (direction == RIGHT)
		translate(-1.0f * right * velocity);
	if (direction == UP)
		translate(up * velocity);
	if (direction == DOWN)
		translate(-1.0f * up * velocity);

	std::cout << "doing movement, position: " << direction << ", cord: " << position.x << ", " << position.y << ", " << position.z << std::endl;
}

void LocalPlayer::doCameraRotation(double xdelta, double ydelta) {
	if ((camera.getEulerAngles().x > 90.0f && ydelta > 0.0f) || (camera.getEulerAngles().x < -90.0f && ydelta < 0.0f)) {
		//ydelta = 0.0f;
	}

	rotate(glm::vec3(0.0f, xdelta * mouseSensitivity, 0.0f));
	
	camera.rotate(glm::vec3(-ydelta * mouseSensitivity, 0.0f, 0.0f));
}

void LocalPlayer::update(GLfloat delta) {
	processKeyboard(delta);
	updateMatrix();
}