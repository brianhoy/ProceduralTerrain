#pragma once
#include <SOIL.h>
#include <string>
#include <GL/glew.h>

class Texture {
public:
	int edgeBehavior;
	GLuint textureId;
	unsigned char* image;

	Texture(const char* path, int width, int height, int channels = 0, int type = SOIL_LOAD_RGB) {
		image = SOIL_load_image(path, &width, &height, &channels, type);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};