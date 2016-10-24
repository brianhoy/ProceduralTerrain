#pragma once
#include "Geometry.h"

class TerrainGeometry : public Geometry {
	int levels = 4;
	float size = 1000.0f;

	/* 
		@param levels How many levels of detail there are
		@param segments How many segments there are in the smallest level of detail
		@param size The size of the largest level of detail
	*/
	TerrainGeometry(int levels, int segments, float size) {
		indices = std::vector<GLuint>();
		vertices = std::vector<Vertex>();

		/* First create all the vertices with size 1 */
		for (int i = 0; i < levels; i++) {
			
		}
	}
};