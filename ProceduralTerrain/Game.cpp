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

	std::shared_ptr<PlaneGeometry> planeGeometry = std::make_shared<PlaneGeometry>(1, 1, 100, 100);
	std::shared_ptr<CubeGeometry> cubeGeometry = std::make_shared<CubeGeometry>();

	Texture tex = Texture("../Content/container.jpg");
	std::shared_ptr<MaterialBasic> mat = std::make_shared<MaterialBasic>();

	std::shared_ptr<Mesh> cubeTestMesh = std::make_shared<Mesh>(cubeGeometry, mat);
	cubeTestMesh->textures.push_back(tex);

	OBJLoader loader = OBJLoader();
	auto model = loader.loadModel("../Content/sponza/sponza.obj");

	//cubeTestMesh->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	//cubeTestMesh->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
	model->setScale(glm::vec3(0.03f, 0.03f, 0.03f));

	//scene.add(cubeTestMesh);
	scene.add(model);


	//Terrain terrain = Terrain(5000, 2, 2);
	//terrain.createTiles();
	//scene.add(terrain.tiles);


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