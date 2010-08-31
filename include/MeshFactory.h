/*
 * MeshFactory.h
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Mesh.h"
#include "ObjLoader.h"
#include "Singleton.h"

class MeshFactory : public Singleton<MeshFactory>{
public:
	Mesh * tetrahedron();
	Mesh * cube();
	Mesh * stars();
	Mesh * plane();
	Mesh * load(string file);
	Mesh * lamp();

private:
	friend class Singleton<MeshFactory>;
	MeshFactory();
	~MeshFactory();

};

