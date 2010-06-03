/*
 * TextureFactory.h
 *
 *  Created on: Mar 25, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <vector>
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#include "Texture.h"

using namespace std;

class TextureFactory {
public:

	vector<Texture*> textures;

	static TextureFactory& Instance()
	 {
	    static TextureFactory singleton;
	    return singleton;
	  }

	void load(string filename, string name);
	void loadCubeMap(string filename, string name);
	void uniforms(GLint program);
private:

	GLenum textureEnums[32];

	TextureFactory();
	virtual ~TextureFactory();
	TextureFactory(const TextureFactory&);                 // Prevent copy-construction
	TextureFactory& operator=(const TextureFactory&);      // Prevent assignment
};

