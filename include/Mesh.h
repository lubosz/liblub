/*
 * Copyright © 2010 Lubosz Sarnecki
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "OpenGL.h"

// TODO(bmonkey): This is a very bad solution (and also hardcoded)
const unsigned maxBuffers = 6;

class Mesh {
 public:
	Mesh();
	void addBuffer(vector<GLfloat> content, unsigned size, string name);
	void addElementBuffer(vector<GLuint> content);
	void setDrawType(GLint drawType);
	void draw();
 private:
	/* Create handles for a Vertex Array Object
	 * and 6 Vertex Buffer Objects */

	GLuint vao, vbo[maxBuffers];
	GLint drawType;

	unsigned indexSize;
	unsigned bufferCount;

	virtual ~Mesh();
};

