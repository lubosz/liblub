/*
 * Copyright © 2010 Lubosz Sarnecki
 * ShaderSystem.h
 *
 *  Created on: Mar 22, 2010
 */

#pragma once

#include "common/Qt3D.h"
#include "Material/Shader.h"
#include "Material/Uniform.h"

class ShaderProgram {
 public:
	ShaderProgram();
	virtual ~ShaderProgram();
	void attachShader(string fileName, GLenum type);
	void attachShader(
	        string fileName, GLenum type, const vector<string> & defines
	);

	void attachVertFrag(string file);
	void attachVertFrag(string file, const vector<string> & defines);
	void attachVertGeom(string file);
	void attachVertFragGeom(string file);

	void use();
	void bindAttribIfUnbound(string name);
	void bindAttrib(string name);
	void bindAttrib(unsigned position, string name);
	void defaultAttribs();
	void init();
	void linkAndUse();
	GLuint getReference() const;

	string name;
	vector<Uniform<float> > uniforms;
	vector<Uniform<int> > uniformsi;
	void setUniform(string name, int value);
	void setUniform(string name, float value);
	void setUniform(string name, const QMatrix3x3 & matrix);
	void setUniform(string name, const QMatrix4x4 & matrix);
	void setUniform(string name, const QVector2D & vector);
	void setUniform(string name, const QVector3D & vector);
	void setUniform(string name, const QVector4D & vector);

	void translateUniformf(unsigned id, const vector<float> & values);

	void initUniforms();
	void bindUniformBuffer(string name, GLuint bindIndex, GLuint bufferHandle);


 private:
	/* This is a handle to the shader program */
	GLuint program;
	template<typename T> void initUniformsByType(vector<Uniform<T> > & uniforms);
	vector<Shader*> shaders;
	vector<string> boundAttribs;
	unsigned attribCount;
	void printProgramInfoLog();
	void detachShader(Shader *shader);
	void reload();
};
