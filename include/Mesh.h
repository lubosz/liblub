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

class Mesh {
public:
	/* Create handles for our Vertex Array Object and three Vertex Buffer Objects */
	GLuint vao, vbo[3];



	Mesh(vector<GLfloat> position, vector<GLfloat> color, vector<GLfloat> normals, vector<GLfloat> uv, vector<GLubyte> index);
	virtual ~Mesh();
	void addBuffer(vector<GLfloat> content, unsigned size);
	void addElementBuffer(vector<GLubyte> content);

private:
	unsigned bufferCount, myASS;
};

