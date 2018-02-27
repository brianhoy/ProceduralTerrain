#pragma once
#include <vector>

#include "MeshCollection.h"
#include "Mesh.h"
#include "Camera.h"
#include "Object3D.h"
#include "Texture.h"

class Scene
{
public:
	Scene() {
		meshCollections.push_back(std::make_shared<MeshCollection>());
	}

	std::vector<std::shared_ptr<Mesh>> renderedMeshes = std::vector<std::shared_ptr<Mesh>>();
	std::vector<std::shared_ptr<MeshCollection>> meshCollections = std::vector<std::shared_ptr<MeshCollection>>();

	long version = 0;

	void add(std::shared_ptr<Mesh> mesh) {
		std::cout << "Adding mesh to scene..." << std::endl;
		meshCollections.front()->meshes.push_back(mesh);
		renderedMeshes.push_back(mesh);
		version++;
	}

	void add(std::shared_ptr<MeshCollection> collection) {
		meshCollections.push_back(collection);
		recursiveAdd(collection.get());
		version++;
	}

	void recursiveAdd(MeshCollection* collection) {
		for (int i = 0; i < collection->meshCollections.size(); i++) {
			recursiveAdd(collection->meshCollections.at(i).get());
		}

		for (int i = 0; i < collection->meshes.size(); i++) {
			renderedMeshes.push_back(collection->meshes.at(i));
		}

	}

	bool remove(int uid) { // returns true if successful
		version++;

		for (int i = 0; i < meshCollections.size(); i++) {
			if (recursiveRemove(meshCollections.at(i), uid)) return true;
		}
		return false;
	}

	bool recursiveRemove(std::shared_ptr<MeshCollection> collection, int uid) {
		for (int i = 0; i < collection->meshes.size(); i++) {
			if (collection->meshes.at(i)->uid == uid) {
				collection->meshes.erase(collection->meshes.begin() + i);
				return true;
			}
		}
		for (int i = 0; i < collection->meshCollections.size(); i++) {
			if (collection->meshCollections.at(i)->uid == uid) {
				collection->meshCollections.erase(collection->meshCollections.begin() + i);
				return true;
			}
			recursiveRemove(collection->meshCollections.at(i), uid);
		}
		return false;
	}
};