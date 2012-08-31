/*
 * Copyright © 2010 Lubosz Sarnecki
 * Geometry.h
 *
 *  Created on: Mar 24, 2010
 */

#pragma once

#include "Mesh/Mesh.h"

class Geometry {
 public:
	static Mesh * tetrahedron(const QList<string> & attributes);
	static Mesh * cube(const QList<string> & attributes);
	static Mesh * plane(const QList<string> & attributes, const QRectF &plane);
	static Mesh * stars(const QList<string> & attributes,
            const QVector3D &resolution,
            const float & density,
            const float & randomness,
            const float & colorIntensity
	);
	static Mesh * spiral(const QList<string> & attributes, int resolution);
	static Mesh * sphere(const QList<string> & attributes, GLdouble radius, GLint slices, GLint stacks);
	static Mesh * icosahedron(const QList<string> & attributes);
};
