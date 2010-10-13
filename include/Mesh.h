/*
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "OpenGL.h"

class Mesh {
public:
	Mesh();
	void addBuffer(vector<GLfloat> content, unsigned size, string name);
	void addElementBuffer(vector<GLuint> content);
	void setDrawType(GLint drawType);
	void draw();
private:
	/* Create handles for our Vertex Array Object and three Vertex Buffer Objects */
	GLuint vao, vbo[maxBuffers];
	GLint drawType;

	unsigned indexSize;
	unsigned bufferCount;

	virtual ~Mesh();
};

