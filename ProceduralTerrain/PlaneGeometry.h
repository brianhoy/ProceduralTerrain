#pragma once
#include "Geometry.h"
#include "Vertex.h"
#include <glm/glm.hpp>
#include <cmath>

/* adapted from https://github.com/mrdoob/three.js/blob/master/src/geometries/PlaneBufferGeometry.js */

class PlaneGeometry : public Geometry {
public:
	unsigned int width;
	unsigned int height;
	unsigned int widthSegments;
	unsigned int heightSegments;

	PlaneGeometry(unsigned int width, unsigned int height, unsigned int widthSegments, unsigned int heightSegments) : 
		width(width), height(height), widthSegments(widthSegments), heightSegments(heightSegments) {

		indices = std::vector<GLuint>();
		vertexData = std::vector<Vertex>();

		float width_half = (float)width / 2.0f;
		float height_half = (float)height / 2.0f;

		float gridX = std::floor(widthSegments);
		float gridY = std::floor(heightSegments);

		float gridX1 = gridX + 1;
		float gridY1 = gridY + 1;

		float segment_width = (float)width / gridX;
		float segment_height = (float)height / gridY;

		for (int iy = 0; iy < gridY1; iy++) {

			float y = iy * segment_height - height_half;

			for (int ix = 0; ix < gridX1; ix++) {

				float x = ix * segment_width - width_half;

				Vertex v = Vertex();

				v.position = glm::vec3(x, -y, 1.0f);
				v.texCoords = glm::vec2(ix / gridX, 1 - (iy / gridY));
				v.normal = glm::vec3(0.0f, 0.0f, 1.0f); /* +1 in z direction is perpendicular to plane */
				
				vertexData.push_back(v);
			}

		}

		for (int iy = 0; iy < gridY; iy++) {

			for (int ix = 0; ix < gridX; ix++) {

				int a = ix + gridX1 * iy;
				int b = ix + gridX1 * (iy + 1);
				int c = (ix + 1) + gridX1 * (iy + 1);
				int d = (ix + 1) + gridX1 * iy;

				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(d);

				indices.push_back(b);
				indices.push_back(c);
				indices.push_back(d);
			}

		}
		needsUpdate = true;
	}

	void update() {
		needsUpdate = true;
	}
};