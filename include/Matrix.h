/*
 * Matrix.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include <vector>

#include <boost/multi_array.hpp>
using namespace std;

class Matrix {
public:
	vector<vector<GLfloat> > data;

	unsigned width, height;
	unsigned elements;

	Matrix(unsigned width, unsigned height);
	virtual ~Matrix();

	void readArray(GLfloat * myArray);
	void writeArray(GLfloat * myArray);
};

