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
	static Mesh * tetrahedron(const QList<string> & attributes);
	static Mesh * cube(const QList<string> & attributes);
	static Mesh * plane(const QList<string> & attributes, const QRectF &plane);
	static Mesh * stars(const QList<string> & attributes,
	        vector<float> & resolution,
	        float density,
	        float randomness,
	        float colorIntensity
	);
	static Mesh * spiral(const QList<string> & attributes, int resolution);
	static Mesh * sphere(const QList<string> & attributes, GLdouble radius, GLint slices, GLint stacks);
	static Mesh * icosahedron(const QList<string> & attributes);
 private:
	float density, randomness;
	int variation;
	static float randomize(float density, float randomness);
};
