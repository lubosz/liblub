/*
 * MeshFactory.h
 *
 *  Created on: Jun 8, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Mesh.h"

class MeshFactory{
public:
	static Mesh * tetrahedron();
	static Mesh * cube();
	static Mesh * stars();
	static Mesh * plane();
	static Mesh * load(string file);
	static Mesh * lamp();
	static Mesh * spiral();
	static Mesh * loadDirect(string file);

};

