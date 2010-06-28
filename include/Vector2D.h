/*
 * Vector2D.h
 *
 *  Created on: Jun 9, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

class Vector2D {
public:

	GLfloat x, y;

	Vector2D(GLfloat x, GLfloat y);
	virtual ~Vector2D();
};

