#include "Game.h"

Game::Game() : camera(glm::vec3(0.0f, 0.0f, 0.0f)), scene(Scene()), renderer(Renderer()), 
	player(LocalPlayer()), inputManager(InputManager(renderer.window))
{
	//renderer.createWindow(WIDTH, HEIGHT);

	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	inputManager.setReceiver(&player);

	addTestMeshes();
}


Game::~Game()
{
}

void Game::addTestMeshes() {
	PlaneGeometry* geometry = new PlaneGeometry(100, 100, 10, 10);
	MeshBasicMaterial* mat = new MeshBasicMaterial();
	Mesh* mesh = new Mesh(geometry, mat);
	std::cout << "mesh indices size" << mesh->geometry->indices.size() << std::endl;

	scene.add(mesh);

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

	std::cout << "plaeyr position x: " << player.position.x << std::endl;
	std::cout << "plaeyr position y: " << player.position.y << std::endl;
	std::cout << "plaeyr position z: " << player.position.z << std::endl;

	renderer.render(&camera, &scene);
}