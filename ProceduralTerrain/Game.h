#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include "LocalPlayer.h"

#include "InputManager.h"

#include "CubeGeometry.h"
#include "PlaneGeometry.h"
#include "Mesh.h"
#include "Texture.h"
#include "MaterialBasic.h"
#include "MeshCollection.h"
#include "OBJLoader.h"

// Window dimensions

class Game
{
public:
	Game();
	~Game();

	const GLuint WIDTH = 800, HEIGHT = 600;

	glm::vec3 lastCamera = glm::vec3(0.0f, 0.0f, 0.0f) , lastPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	void start();
	void loop();

	void addTestMeshes();
private:
	Renderer renderer;
	Camera camera;
	Scene scene;
	LocalPlayer player;

	InputManager inputManager;

	GLfloat currentFrame;
	GLfloat deltaTime;
	GLfloat lastFrame;

	Mesh* testmesh;
};

