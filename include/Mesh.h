/*
 * Mesh.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include <vector>

const unsigned maxBuffers = 7;

class Mesh {
public:
	/* Create handles for our Vertex Array Object and three Vertex Buffer Objects */
	GLuint vao, vbo[maxBuffers];
	unsigned indexSize;
	GLint drawType;


	//Mesh(vector<GLfloat> position, vector<GLfloat> color, vector<GLfloat> normals, vector<GLfloat> binormals, vector<GLfloat> tangents, vector<GLfloat> uv, vector<GLuint> index);
	Mesh();
	virtual ~Mesh();
	void addBuffer(vector<GLfloat> content, unsigned size, string name);
	void addElementBuffer(vector<GLuint> content);

	void setDrawType(GLint drawType);
	void draw();

private:
	unsigned bufferCount;
};

