/*
 * Material.h
 *
 *  Created on: Mar 25, 2010
 *      Author: bmonkey
 */

#pragma once

#include <iostream>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "TextureFactory.h"

#include <boost/foreach.hpp>

using namespace std;

class Material {
public:
    ShaderProgram *getShaderProgram() const;
//private:
	ShaderProgram * shaderProgram;
	vector<Texture*> textures;

	virtual void uniforms() = 0;

	void init();

	void addTexture(Texture * texture);
	void addTexture(string file,string name);
	void addTextureCube(string file,string name);

	void defaultAttribs();

	void attachVertFrag(string file);
	void attachVertGeom(string file);

	void attachVertFragGeom(string file);
	void done();

	void initUniforms();
	void activateTextures();

	void activate();
};





