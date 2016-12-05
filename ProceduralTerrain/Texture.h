#pragma once
#include <SOIL.h>
#include <string>
#include <GL/glew.h>

class Texture {
public:
	int edgeBehavior;
	GLuint textureId;
	unsigned char* image;
	int width, height;

	std::string path;
	std::string type;

	int soilType;
	int channels;


	Texture(std::string path = "", std::string type = "diffuse", int channels = 0, int soilType = SOIL_LOAD_RGB) :
		width(width), height(height), channels(channels), type(type), soilType(soilType), path(path) 
	{
	}

	void loadImageData() {
		std::cout << "path: " << path << std::endl;
		image = SOIL_load_image(path.c_str(), &width, &height, &channels, soilType);
	}

	void freeImageData() {
		SOIL_free_image_data(image);
	}
};