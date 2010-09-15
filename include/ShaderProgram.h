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
#include "Matrix.h"
#include "Shader.h"

class ShaderProgram {
public:
	ShaderProgram();
	void attachShader(string fileName, GLenum type);
	void use();
	void bindAttribIfUnbound(string name);
	void bindAttrib(string name);
	void bindAttrib(unsigned position, string name);

	void linkAndUse();
	void setLightPosition(const QVector3D& position);
	GLuint getReference() const;

	void setUniform(const QMatrix3x3 & matrix, string name);
	void setUniform(const QMatrix4x4 & matrix, string name);
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
