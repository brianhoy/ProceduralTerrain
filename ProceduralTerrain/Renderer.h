#pragma once

#include <iostream>
#include <ctime>
#include <chrono>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Scene.h"
#include "InputReceiverInterface.h"
#include "MaterialBasicUploader.h"
#include "MeshCollection.h"

class Renderer
{
public:
	Renderer();
	
	int screenWidth, screenHeight;

	int frameNumber;
	GLuint lastBoundMeshId;

	long lastVersion = -1;

	int createWindow(int width, int height);
	void updateScene(Scene* scene);
	void render(Camera* camera, Scene* scene);
	void renderMeshCollectionRecursive(Camera * camera, MeshCollection * collection);
	void terminate();
	GLFWwindow* window;
	bool shouldClose();
private:
	void uploadGeometry(Geometry* geometry);
	void uploadMaterial(Material* material);
	void uploadTexture(Texture* texture);

	// FIx textures being uplaoded twice and black screen
	void bindTextures(std::vector<Texture>* textures, GLuint program);
	void unbindTextures(std::vector<Texture> textures);

	void initializeTextureArray();
	void addTextureToTextureArray(Texture* texture);
	void generateTextureArrayMips();


	GLuint createProgram(std::vector<GLuint> shaders);
	void initializeUniformBuffer();

	void updateProjectionMatrix(glm::mat4 projection);
	void updateViewMatrix(glm::mat4 view);


	GLuint uboMatrices;
	// store the textures currently loaded in the GPU to know not to make duplicates
	// the int keeps track of how many instance are kept. when it hits 0, the texture is unloaded
	std::vector<std::pair<Texture, int>> loadedTextures = std::vector<std::pair<Texture, int>>(); 
	std::vector<GLuint> boundTextures = std::vector<GLuint>(32);
	// textures stored in texture array
	std::vector<std::pair<Texture, std::string>> arrayTextures = std::vector<std::pair<Texture, std::string>>();

	GLuint arrayTexture;
	int currentSpotInArrayTexture = 0;

	MaterialBasicUploader materialBasicUploader;
};

