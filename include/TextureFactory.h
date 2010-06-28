/*
 * TextureFactory.h
 *
 *  Created on: Mar 25, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <vector>
#include "common.h"
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
	void splatTexture(string name, int resolution);
	void uniforms(GLint program);
private:

	GLenum textureEnums[32];

	TextureFactory();
	virtual ~TextureFactory();
	TextureFactory(const TextureFactory&);                 // Prevent copy-construction
	TextureFactory& operator=(const TextureFactory&);      // Prevent assignment
};

