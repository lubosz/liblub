/*
 * ShaderSystem.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QVector3D>

#include <string>
#include <list>

#include "common.h"
#include "Shader.h"

class ShaderProgram {
public:
	ShaderProgram();
	void attachShader(string fileName, GLenum type);
	void attachShader(string fileName, GLenum type, const vector<string> & defines);
	void use();
	void bindAttribIfUnbound(string name);
	void bindAttrib(string name);
	void bindAttrib(unsigned position, string name);

	void linkAndUse();
	GLuint getReference() const;

	void setUniform(float value, string name);
	void setUniform(const QMatrix3x3 & matrix, string name);
	void setUniform(const QMatrix4x4 & matrix, string name);
	void setUniform(const QVector3D & vector, string name);
	void setUniform(const QVector4D & vector, string name);
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
