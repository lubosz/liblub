/*
 * ShaderSystem.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once


#include <string>
#include <list>

#include "common.h"

#include "Shader.h"

using namespace std;

class ShaderProgram {
public:
	ShaderProgram();
	void attachShader(string fileName, GLenum type);
	void use();
	void bindAttribIfUnbound(string name);
	void bindAttrib(string name);
	void bindAttrib(unsigned position, string name);

	void linkAndUse();
	void setNormalMatrix(GLfloat * modelmatrix);
	void setModelViewMatrix(GLfloat * mvMatrix);
	void setModelViewProjectionMatrix(GLfloat * mvpMatrix);
	void setProjectionMatrix(GLfloat * pMatrix);
	void setLightPosition(GLfloat x, GLfloat y, GLfloat z);
	GLuint getReference() const;
private:
	/* This is a handle to the shader program */
	GLuint program;
	list<Shader*> shaders;
	list<string> boundAttribs;
	unsigned attribCount;
	virtual ~ShaderProgram();
	void printProgramInfoLog();
	void detachShader(Shader *shader);
	void reload();
};
