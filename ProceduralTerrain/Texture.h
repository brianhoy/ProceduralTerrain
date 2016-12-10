#pragma once
#include <SOIL.h>
#include <string>
#include <GL/glew.h>

class Texture {
public:
	int edgeBehavior;
	GLuint textureId;

	GLint textureArrayLocation = -1;

	unsigned char* image;
	int width, height;

	std::string path;
	std::string type;

	int soilType;
	int channels;

	bool needsUpdate;
	bool loaded = false;

	Texture(std::string path = "", std::string type = "diffuse", int channels = 0, int soilType = SOIL_LOAD_RGB) :
		width(width), height(height), channels(channels), type(type), soilType(soilType), path(path) {
		needsUpdate = true;
	}

	void loadImageData() {
		std::cout << "path: " << path << std::endl;
		image = SOIL_load_image(path.c_str(), &width, &height, &channels, soilType);
		loaded = true;
	}

	void freeImageData() {
		SOIL_free_image_data(image);
		loaded = false;
	}
};