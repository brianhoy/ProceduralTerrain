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
		meshCollections.push_back(MeshCollection());
	}

	std::vector<MeshCollection> meshCollections = std::vector<MeshCollection>();


	void add(Mesh mesh) {
		meshCollections.front().meshes.push_back(mesh);
	}

	void add(MeshCollection collection) {
		meshCollections.push_back(collection);
	}

	bool remove(int uid) { // returns true if successful
		for (int i = 0; i < meshCollections.size(); i++) {
			if (recursiveRemove(&meshCollections.at(i), uid)) return true;
		}
		return false;
	}

	bool recursiveRemove(MeshCollection* collection, int uid) {
		for (int i = 0; i < collection->meshes.size(); i++) {
			if (collection->meshes.at(i).uid == uid) {
				collection->meshes.erase(collection->meshes.begin() + i);
				return true;
			}
		}
		for (int i = 0; i < collection->meshCollections.size(); i++) {
			if (collection->meshCollections.at(i).uid == uid) {
				collection->meshCollections.erase(collection->meshCollections.begin() + i);
				return true;
			}
			recursiveRemove(&collection->meshCollections.at(i), uid);
		}
		return false;
	}
};