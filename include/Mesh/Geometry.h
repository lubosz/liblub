/*
 * Copyright © 2010 Lubosz Sarnecki
 * Geometry.h
 *
 *  Created on: Mar 24, 2010
 */

#pragma once

#include "Mesh.h"

class Geometry {
 public:
	static Mesh * tetrahedron();
	static Mesh * cube();
	static Mesh * plane(const QRectF &plane);
	static Mesh * stars(
	        vector<float> & resolution,
	        float density,
	        float randomness,
	        float colorIntensity
	);
	static Mesh * spiral(int resolution);
	static Mesh * sphere(GLdouble radius, GLint slices, GLint stacks);
	static Mesh * icosahedron();
 private:
	float density, randomness;
	int variation;
	static float randomize(float density, float randomness);
};
