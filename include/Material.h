/*
 * Material.h
 *
 *  Created on: Mar 25, 2010
 *      Author: bmonkey
 */

#pragma once

#include <iostream>
#include "ShaderProgram.h"

using namespace std;

class Material {
public:
	ShaderProgram * shaderProgram;

	//Material(){}

	//virtual void uniforms();
	virtual void uniforms() = 0;

	void defaultAttribs(){
		shaderProgram->bindAttrib("in_Position");
		shaderProgram->bindAttrib("in_Color");
		shaderProgram->bindAttrib("in_Normal");
		shaderProgram->bindAttrib("in_Binormal");
		shaderProgram->bindAttrib("in_Tangent");
		shaderProgram->bindAttrib("in_Uv");
	}
};


