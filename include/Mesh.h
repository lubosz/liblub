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
#include <vector>

using namespace std;

const unsigned maxBuffers = 7;

class Mesh {
public:
	/* Create handles for our Vertex Array Object and three Vertex Buffer Objects */
	GLuint vao, vbo[maxBuffers];
	unsigned indexSize;



	Mesh(vector<GLfloat> position, vector<GLfloat> color, vector<GLfloat> normals, vector<GLfloat> binormals, vector<GLfloat> tangents, vector<GLfloat> uv, vector<GLuint> index);
	virtual ~Mesh();
	void addBuffer(vector<GLfloat> content, unsigned size);
	void addElementBuffer(vector<GLuint> content);

private:
	unsigned bufferCount;
};

