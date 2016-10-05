#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include "LocalPlayer.h"

#include "InputManager.h"

// Window dimensions

class Game
{
public:
	Game();
	~Game();

	const GLuint WIDTH = 800, HEIGHT = 600;

	void start();
	void loop();
private:
	Renderer renderer;
	Camera camera;
	Scene scene;
	LocalPlayer player;

	InputManager inputManager;

	GLfloat currentFrame;
	GLfloat deltaTime;
	GLfloat lastFrame;
};

