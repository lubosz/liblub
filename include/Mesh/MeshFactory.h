/*
 * Copyright © 2010 Lubosz Sarnecki
 * MeshFactory.h
 *
 *  Created on: Jun 8, 2010
 */

#pragma once

#include "Mesh.h"

class MeshFactory {
 public:
	static Mesh * tetrahedron();
	static Mesh * cube();
	static Mesh * stars(
	        float resolution, float density, float randomness, float colorIntensity
	);
	static Mesh * plane();
	static Mesh * load(string file);
	static Mesh * load(string file, GLint drawType);
	static Mesh * spiral();
	static Mesh * loadDirect(string file);
};

