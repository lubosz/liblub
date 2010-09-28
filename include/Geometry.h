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
	static Mesh * makeTetrahedron();
	static Mesh * makeCube();
	static Mesh * makePlane();
	static Mesh * makeStars(vector<float> resolution, float density, float randomness);
	static Mesh * makeSpiral(int resolution, float density, float randomness);
private:
	float density, randomness;
	int variation;
	static float randomize(float density, float randomness);
};
