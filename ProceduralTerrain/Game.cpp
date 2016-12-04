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
	PlaneGeometry* planeGeometry = new PlaneGeometry(1, 1, 10, 10);
	CubeGeometry* cubeGeometry = new CubeGeometry();

	Texture* tex = new Texture("../Content/container.jpg");
	MaterialBasic* mat = new MaterialBasic(tex);

	testmesh = new Mesh(planeGeometry, mat);
	Mesh* testmesh2 = new Mesh(cubeGeometry, mat);

	//std::cout << "mesh indices size" << testmesh->geometry->indices.size() << std::endl;

	//testmesh->setScale(glm::vec3(1000.0f, 1000.0f, 1000.0f));

	testmesh->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));

	scene.add(testmesh2);
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
		glm::vec3 pos2 = player.camera.getPosition();
		std::cout << "Camera Position xyz: " << pos2.x << ", " << pos2.y << ", " << pos2.z << std::endl;
		pos2 = player.camera.getFront();
		std::cout << "Camera Front: " << pos2.x << ", " << pos2.y << ", " << pos2.z << std::endl;
		pos2 = player.camera.getUp();
		std::cout << "Camera Up: " << pos2.x << ", " << pos2.y << ", " << pos2.z << std::endl;
		lastPosition = pos;
	}
	if (ang != lastCamera) {
		std::cout << "Camera Pitch: " << ang.x << ", Yaw: " << ang.y << ", Roll: " << ang.z << std::endl;
		glm::vec3 pos2 = player.camera.getPosition();
		std::cout << "Camera Position xyz: " << pos2.x << ", " << pos2.y << ", " << pos2.z << std::endl;
		pos2 = player.camera.getFront();
		std::cout << "Camera Front: " << pos2.x << ", " << pos2.y << ", " << pos2.z << std::endl;
		pos2 = player.camera.getUp();
		std::cout << "Camera Up: " << pos2.x << ", " << pos2.y << ", " << pos2.z << std::endl;
		lastCamera = ang;
	}

	//player.update();

	renderer.render(&player.camera, &scene);
}