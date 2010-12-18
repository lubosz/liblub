/*
 * ShaderSystem.h
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Qt3D.h"
#include "Shader.h"
#include "Uniform.h"

class ShaderProgram {
public:
	ShaderProgram();
	virtual ~ShaderProgram();
	void attachShader(string fileName, GLenum type);
	void attachShader(string fileName, GLenum type, const vector<string> & defines);

	void attachVertFrag(string file);
	void attachVertFrag(string file, const vector<string> & defines);
	void attachVertGeom(string file);
	void attachVertFragGeom(string file);

	void use();
	void bindAttribIfUnbound(string name);
	void bindAttrib(string name);
	void bindAttrib(unsigned position, string name);
	void defaultAttribs();

	void linkAndUse();
	GLuint getReference() const;

	string name;
	vector<Uniform> uniforms;
	vector<Uniformi> uniformsi;
	void setUniform(float value, string name);
	void setUniform(const QMatrix3x3 & matrix, string name);
	void setUniform(const QMatrix4x4 & matrix, string name);
	void setUniform(const QVector2D & vector, string name);
	void setUniform(const QVector3D & vector, string name);
	void setUniform(const QVector4D & vector, string name);

	void initUniforms();


private:
	/* This is a handle to the shader program */
	GLuint program;
	list<Shader*> shaders;
	list<string> boundAttribs;
	unsigned attribCount;
	void printProgramInfoLog();
	void detachShader(Shader *shader);
	void reload();
};
