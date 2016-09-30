#include "Game.h"

Game::Game() : camera(glm::vec3(0.0f, 0.0f, 0.0f)), scene(Scene()), renderer(Renderer()), 
	player(LocalPlayer()), inputManager(InputManager(renderer.window))
{
	//renderer.createWindow(WIDTH, HEIGHT);

	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	inputManager.setReceiver(&player);
}


Game::~Game()
{
}

void Game::start() {
	// Game loop
	while (!(renderer.shouldClose()))
	{
		loop();
	}
}

void Game::loop() {
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	player.doMovement(deltaTime);
	renderer.render(camera, scene);
}