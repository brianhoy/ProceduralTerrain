#pragma once
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include "Vertex.h"
#include <glm/gtc/matrix_inverse.hpp>

class Renderer;

class Geometry {
	friend class Renderer;
public:
	bool needsUpdate = true;
	std::vector<Vertex> vertexData = std::vector<Vertex>();
	std::vector<GLuint> indices = std::vector<GLuint>();
	GLuint VAO;

	void applyMatrix(glm::mat4 matrix) {
		for (int i = 0; i < vertexData.size(); i++) {
			glm::vec3 vertex = vertexData.at(i).position;
			glm::vec4 vertex4 = matrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0);
			vertexData.at(i).position = glm::vec3(vertex4.x, vertex4.y, vertex4.z);
		}

		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(matrix));

		for (int i = 0; i < vertexData.size(); i++) {
			vertexData.at(i).normal = normalMatrix * vertexData.at(i).normal;
		}
	}
	void calculateVertexNormals() {


		struct NormalCalculationData {
			glm::vec3 normal;
			double magnitude;
		};

		for (GLuint vertexIndex = 0; vertexIndex < vertexData.size(); vertexIndex++) {

			auto calcs = std::vector<NormalCalculationData>();
			auto adjacentIndiceIndices = getAdjacentTriangleIndiceIndexes(vertexIndex);
			double totalMagnitude = 0;

			// For each adjacent triangle calculate the perpendicular 
			for (GLuint i = 0;  i < adjacentIndiceIndices.size(); i++) {

				glm::vec3 A = vertexData.at(indices.at(adjacentIndiceIndices.at(i    ))).position;
				glm::vec3 B = vertexData.at(indices.at(adjacentIndiceIndices.at(i + 1))).position;
				glm::vec3 C = vertexData.at(indices.at(adjacentIndiceIndices.at(i + 2))).position;

				calcs.push_back(NormalCalculationData());

				glm::vec3 cross = glm::cross(B - A, C - A);
				calcs.back().normal = glm::normalize(cross);
				calcs.back().magnitude = glm::length(cross);
				totalMagnitude += calcs.back().magnitude;
			}

			// Now average out the normals using the weights
			glm::vec3 normal = glm::vec3();
			for (GLuint i = 0; i < calcs.size(); i++) {
				GLfloat weight = calcs.at(i).magnitude / totalMagnitude;
				normal += weight * calcs.at(i).normal;
			}

			// Finally, set the actual vertex data
			vertexData.at(vertexIndex).normal = glm::normalize(normal);
		}
	}

private:
	GLuint VBO, EBO; // only the VAO needs to be seen by the renderer
	/* 
		What is an adjacent vertex?
			A vertex that is connected by an edge
		How do you detect if a vertex is connected by an edge?
			If two indices (sets of 3 vertices) both have the same Vertex A, every other vertex in both those indices
			is connected to the Vertex A by an edge
	*/
	std::vector<GLuint> getAdjacentTriangleIndiceIndexes(GLuint vertexIndex) {
		std::vector<GLuint> adjacentVertexIndices = std::vector<GLuint>();
		// Loop through every index3
		for (GLuint vertexIndexL = 0; vertexIndexL < indices.size(); vertexIndexL += 3) 
		{
			// Loop through every vertex of this index3
			for (GLuint vertexIndexA = vertexIndexL; vertexIndexA < vertexIndexL + 3; vertexIndexA++) 
			{
				// If that index3 has the vertexIndex
				if (indices.at(vertexIndexA == vertexIndex))
				{
					// Add that index3 triangle to the vec3
					adjacentVertexIndices.push_back(vertexIndexL);
					
					// We do not need to check other vertex indices in this index3 to see if they're the one in the parameter, 
					// since no index3 has the same two indices, so the other indices are definitely not the one in the parameter
					break;
				}
			}			
		}
	}
};