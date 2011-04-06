/*
 * Copyright © 2010 Lubosz Sarnecki
 * Material.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include "ShaderProgram.h"
#include "TextureFactory.h"


// using namespace std;

class Material {
 public:
    virtual ~Material() {}
    ShaderProgram *getShaderProgram();
	void activate();
	void samplerUniforms();
	void addTexture(Texture * texture);
	void done();

	vector<Texture*> textures;

	ShaderProgram * shaderProgram;
	void init();
 protected:
	QVector4D diffuseColor;

	void addTexture(string file, string name);
	void addTextureCube(string file, string name);

 private:
	virtual void uniforms() = 0;
	void defaultAttribs();
	void activateTextures();
};





