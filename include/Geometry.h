/*
 * Geometry.h
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Mesh.h"

class Geometry {
public:
	float density, randomness;
	int variation;

	static Mesh * makeTetrahedron();
	static Mesh * makeCube();
	static Mesh * makeStars(vector<float> resolution, float density, float randomness, int variation);
	static float randomize(float density, float randomness, int variation);
};
