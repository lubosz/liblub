/*
 * ObjLoader.h
 *
 *  Created on: Jun 2, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "Mesh.h"
const string meshDir = "media/meshes/";

class ObjLoader {
public:

	static ObjLoader& Instance()
	 {
	    static ObjLoader singleton;
	    return singleton;
	  }

	void readFile(string file);

	Mesh* mesh;
	string objSource;

private:
	ObjLoader();                                 // Private constructor
	~ObjLoader();
	ObjLoader(const ObjLoader&);                 // Prevent copy-construction
	ObjLoader& operator=(const ObjLoader&);      // Prevent assignment
};
