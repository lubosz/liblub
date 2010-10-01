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
	void activate();
	void initUniforms();
	void addTexture(Texture * texture);
	void done();
protected:
	ShaderProgram * shaderProgram;
	QVector4D diffuseColor;
	void init();


	void addTexture(string file,string name);
	void addTextureCube(string file,string name);

	void attachVertFrag(string file);
	void attachVertGeom(string file);
	void attachVertFragGeom(string file);


private:

	vector<Texture*> textures;
	virtual void uniforms() = 0;
	void defaultAttribs();
	void activateTextures();
};





