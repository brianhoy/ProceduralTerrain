#pragma once
#include "Mesh.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "MaterialBasic.h"

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

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Data to fill
		Geometry* geometry = new Geometry();
		std::vector<Texture> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; 

			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
			{
				vertex.texCoords = glm::vec2(0.0f, 0.0f);
			}
			geometry->vertexData.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				geometry->indices.push_back(face.mIndices[j]);
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		Mesh mesh_ = Mesh(geometry, material);
		
		return Mesh(geometry, material);

	}

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;

			std::string filename = std::string(str.C_Str());
			filename = directory + '/' + filename;

			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == filename)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				Texture texture = Texture(filename, typeName);

				// Store it as texture loaded for entire model, to ensure we won't unneccessary load duplicate textures.
				textures.push_back(texture);
				this->textures_loaded.push_back(texture); 
			}
		}
		return textures;
	}
};

