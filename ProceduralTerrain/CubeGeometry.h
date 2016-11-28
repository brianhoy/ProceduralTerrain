#pragma once
#include "Geometry.h"

class CubeGeometry : public Geometry {
public:
	CubeGeometry() {
		/*int arr_indices[] = { 0, 1, 2, 0, 2, 3, //front
			4, 5, 6, 4, 6, 7, //right
			8, 9, 10, 8, 10, 11, //back
			12, 13, 14, 12, 14, 15, //left
			16, 17, 18, 16, 18, 19, //upper
			20, 21, 22, 20, 22, 23 }; //bottom
		indices = std::vector<GLuint>(arr_indices, arr_indices + sizeof(arr_indices) / sizeof(int));

		std::cout << "indices contains " << std::endl;
		for (GLuint indice : indices) {
			std::cout << indice << ", ";
		}

		vertices = std::vector<Vertex>();

		//front
		vertices.push_back(Vertex(glm::vec3(-1.0, -1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, -1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, 1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, 1.0, 1.0)));

		//right
		vertices.push_back(Vertex(glm::vec3(1.0, 1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, 1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, -1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, -1.0, 1.0)));

		//back
		vertices.push_back(Vertex(glm::vec3(-1.0, -1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, -1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, 1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, 1.0, -1.0)));

		//left
		vertices.push_back(Vertex(glm::vec3(-1.0, -1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, -1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, 1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, 1.0, -1.0)));

		//upper
		vertices.push_back(Vertex(glm::vec3(1.0, 1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, 1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, 1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, 1.0, -1.0)));

		//bottom
		vertices.push_back(Vertex(glm::vec3(-1.0, -1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, -1.0, -1.0)));
		vertices.push_back(Vertex(glm::vec3(1.0, -1.0, 1.0)));
		vertices.push_back(Vertex(glm::vec3(-1.0, -1.0, 1.0))); */

		vertexData = {
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
			Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),

			Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(-0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),

			Vertex(glm::vec3(-0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),

			Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),

			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, -0.5f, 0.5f)),
			Vertex(glm::vec3(-0.5f, -0.5f, 0.5f)),
			Vertex(glm::vec3(-0.5f, -0.5f, -0.5f)),

			Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, -0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(-0.5f, 0.5f, 0.5f)),
			Vertex(glm::vec3(-0.5f, 0.5f, -0.5f)),

		};

		indices = {};
		needsUpdate = true;
	}
};