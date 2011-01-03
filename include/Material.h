/*
 * Material.h
 *
 *  Created on: Mar 25, 2010
 *      Author: bmonkey
 */

#pragma once

#include "ShaderProgram.h"
#include "TextureFactory.h"


using namespace std;

class Material {
public:
    ShaderProgram *getShaderProgram();
	void activate();
	void samplerUniforms();
	void addTexture(Texture * texture);
	void done();

	vector<Texture*> textures;

	ShaderProgram * shaderProgram;
protected:
	QVector4D diffuseColor;
	void init();


	void addTexture(string file,string name);
	void addTextureCube(string file,string name);




private:
	virtual void uniforms() = 0;
	void defaultAttribs();
	void activateTextures();
};





