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
	PlaneGeometry* geometry = new PlaneGeometry(1, 1, 10, 10);
	//CubeGeometry* cubeGeometry = new CubeGeometry();

	MeshBasicMaterial* mat = new MeshBasicMaterial();
	//testmesh = new Mesh(cubeGeometry, mat);
	testmesh = new Mesh(geometry, mat);
	std::cout << "mesh indices size" << testmesh->geometry->indices.size() << std::endl;

	//testmesh->setScale(glm::vec3(1000.0f, 1000.0f, 1000.0f));
	scene.add(testmesh);

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

	player.update(deltaTime);
	//testmesh->rotate(glm::vec3(deltaTime, deltaTime, deltaTime));

	glm::vec3 ang = player.camera.getEulerAngles();
	glm::vec3 pos = player.getPosition();

	if (pos != lastPosition) {
		std::cout << "Player Position xyz: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
		lastPosition = pos;
	}
	if (ang != lastCamera) {
		std::cout << "Camera Pitch: " << ang.x << ", Yaw: " << ang.y << ", Roll: " << ang.z << std::endl;
		lastCamera = ang;
	}

	renderer.render(&player.camera, &scene);
}