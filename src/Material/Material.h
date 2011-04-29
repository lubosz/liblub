/*
 * Copyright © 2010 Lubosz Sarnecki
 * Material.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include "ShaderProgram.h"
#include "Texture.h"


// using namespace std;

class Material {
 public:
    virtual ~Material() {}
    ShaderProgram *getShaderProgram();
	void activate();
	void samplerUniforms();
	void addTexture(Texture * texture);
	void done(const QList<string> & attributes);

	vector<Texture*> textures;

	ShaderProgram * shaderProgram;
	void init();
	void activateTextures();
 protected:
	QVector4D diffuseColor;

	void addTexture(string file, string name);
	void addTextureCube(string file, string name);

 private:
	virtual void uniforms() = 0;
	void defaultAttribs();
};





