#pragma once
#include "Geometry.h"
#include "Vertex.h"
#include <glm/glm.hpp>

class PlaneGeometry : public Geometry {
	unsigned int width;
	unsigned int height;
	unsigned int widthSegments;
	unsigned int heightSegments;

	PlaneGeometry(unsigned int width, unsigned int height, unsigned int widthSegments, unsigned int heightSegments) : 
		width(width), height(height), widthSegments(widthSegments), heightSegments(heightSegments) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				Vertex v = Vertex();
				v.Position = 
				vertices.push_back()
			}
		}
	}
};