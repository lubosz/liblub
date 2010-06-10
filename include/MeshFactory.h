/*
 * MeshFactory.h
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Mesh.h"
#include "ObjLoader.h"

class MeshFactory {
public:

	static MeshFactory& Instance()
	 {
	    static MeshFactory singleton;
	    return singleton;
	  }

	void drawMeshes();
	void tetrahedron();
	void addMesh();
	void loadObj(string file);

	vector<Mesh*> meshes;

private:
	MeshFactory();                                 // Private constructor
	~MeshFactory();
	MeshFactory(const MeshFactory&);                 // Prevent copy-construction
	MeshFactory& operator=(const MeshFactory&);      // Prevent assignment
};

