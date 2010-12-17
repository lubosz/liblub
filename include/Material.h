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
    ShaderProgram *getShaderProgram() const;
	void activate();
	void initUniforms();
	void addTexture(Texture * texture);
	void done();
	vector<Texture*> textures;

protected:
	ShaderProgram * shaderProgram;
	QVector4D diffuseColor;
	void init();


	void addTexture(string file,string name);
	void addTextureCube(string file,string name);




private:
	virtual void uniforms() = 0;
	void defaultAttribs();
	void activateTextures();
};





