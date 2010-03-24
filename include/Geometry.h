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
};
