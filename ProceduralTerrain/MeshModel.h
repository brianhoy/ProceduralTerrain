#pragma once
#include "Mesh.h"
#include "Texture.h"

class MeshModel : public Mesh 
{
public:
	MeshModel(GLchar* path, MaterialBasic* material)
	{
		noDraw = true;
		this->material = material;
		this->materialBasic = material;
		loadModel(path);

	}

	MaterialBasic* materialBasic = nullptr;

	std::string directory;
	std::vector<Texture> textures_loaded;
private:
	void loadModel(std::string path) {
	}





};

