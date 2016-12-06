#pragma once
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MaterialBasic.h"

class OBJLoader
{
public:
	void loadModel(std::string path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		directory = "../Content"; //path.substr(0, path.find_last_of('/'));

		this->processNode(scene->mRootNode, scene);

	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		// Process all the node's meshes (if any)
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

			Mesh mesh = processMesh(aiMesh, scene);
			add(mesh);
			//std::cout << "mesh parent: " << mesh.getParent();

			children.push_back(mesh);
		}

		// Then do the same for each of its children
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{

			// NOTE TO SELF
			/*
			Each node contains multiple meshes. mChildren =/= mNumMeshes
			So you need to make a special type of mesh that can be rendered called MeshCollection
			that resembles an aiNode

			That way you can properly mirror the tree

			Or you can just add every single mesh child to the children array. Not sure which is better
			Probably MeshCollection
			*/

			this->processNode(node->mChildren[i], scene);
		}

		for (int i = 0; i < textures_loaded.size(); i++) {
			materialBasic->textures.push_back(&textures_loaded.at(i));
		}

		//materialBasic->textures.insert(materialBasic->textures.end(), textures_loaded.begin(), textures_loaded.end());
	}

};