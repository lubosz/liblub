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
#include "Material.h"

class ShaderFactory {
public:
	vector<Shader> shaders;
	vector<ShaderProgram> programs;

	typedef std::vector<Material*>  materialList;
	materialList materials;

	static ShaderFactory& Instance()
	 {
	    static ShaderFactory singleton;
	    return singleton;
	  }

	void defaultAttribs();
	void addUniforms();

	void vertexColorShader();
	void brickShader();
	void textureShader();
	void multiTextureShader();
	void earthShader();
	void coneMapShader();
	void procBump();
	void convolution();
	void bump1();
	void bump2();
	void relief();
	void ocean();
	void particle();
	void env();
	void normalcolor();
	void stars();

private:
	ShaderFactory();
	virtual ~ShaderFactory();
	ShaderFactory(const ShaderFactory&);                 // Prevent copy-construction
	ShaderFactory& operator=(const ShaderFactory&);      // Prevent assignment

};
