/*
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once




/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include "Matrix.h"

class Mesh {
public:
	/* Create handles for our Vertex Array Object and three Vertex Buffer Objects */
	GLuint vao, vbo[3];

	//Matrix vertices;
	//Matrix vertexColors;

	unsigned bufferCount;

	Mesh();
	virtual ~Mesh();
	void makeTetrahedron();
	void fillBuffer(vector<GLfloat> content, GLenum target);
};

