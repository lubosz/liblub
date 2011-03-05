/*
 * Copyright © 2010 Lubosz Sarnecki
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "common/OpenGL.h"
#include <string>
#include <vector>
#include <QVector3D>
#include "Mesh/AABB.h"

using std::string;
using std::vector;

// TODO(bmonkey): This is a very bad solution (and also hardcoded)
const unsigned maxBuffers = 6;

class Mesh {
 public:
	Mesh();
	void addBuffer(vector<GLfloat> content, unsigned size, string name);
	void addElementBuffer(vector<GLuint> content);
	void setDrawType(GLint drawType);
	void draw();

	AABB * boundingBox;


 private:
	/* Create handles for a Vertex Array Object
	 * and 6 Vertex Buffer Objects */

	GLuint vao, vbo[maxBuffers];
	GLint drawType;

	unsigned indexSize;
	unsigned bufferCount;

	virtual ~Mesh();
};

