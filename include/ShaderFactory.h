/*
 * ShaderFactory.h
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#pragma once

#include <vector>
#include "Shader.h"
#include "ShaderProgram.h"

class ShaderFactory {
public:
	vector<Shader> shaders;
	vector<Shader> programs;

	static ShaderFactory& Instance()
	 {
	    static ShaderFactory singleton;
	    return singleton;
	  }

	void addUniforms();

	void vertexColorShader();
	void brickShader();
	void textureShader();
	void multiTextureShader();
	void earthShader();
	void coneMapShader();

private:
	ShaderFactory();
	virtual ~ShaderFactory();
	ShaderFactory(const ShaderFactory&);                 // Prevent copy-construction
	ShaderFactory& operator=(const ShaderFactory&);      // Prevent assignment

};
