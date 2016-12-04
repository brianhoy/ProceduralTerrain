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

	const char * path;
	int type;
	int channels;


	Texture(const char* path, int channels = 0, int type = SOIL_LOAD_RGB) :
		width(width), height(height), channels(channels), type(type), path(path) {
	}

	void loadImageData() {
		image = SOIL_load_image(path, &width, &height, &channels, type);
	}

	void freeImageData() {
		SOIL_free_image_data(image);
	}
};