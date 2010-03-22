/*
 * RenderEngine.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#include <iostream>

#include "ShaderProgram.h"

class RenderEngine {
public:

	GLuint vao, vbo[3]; /* Create handles for our Vertex Array Object and three Vertex Buffer Objects */
	GLfloat projectionmatrix[16]; /* Our projection matrix starts with all 0s */
	GLfloat modelmatrix[16]; /* Our model matrix  */

	ShaderProgram * shaderProgram;

	unsigned frameCount;

	RenderEngine();
	virtual ~RenderEngine();
	void display();
	void glError(string file, int line);
	void checkVersion();
	GLboolean QueryExtension(char *extName);
};
