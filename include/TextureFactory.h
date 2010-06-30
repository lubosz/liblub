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
#include "Singleton.h"

using namespace std;

class TextureFactory : public Singleton<TextureFactory>{
public:
	friend class Singleton<TextureFactory>;

	vector<Texture*> textures;

	Texture * load(string filename, string name);
	Texture * loadCubeMap(string filename, string name);
	Texture * splatTexture(string name, int resolution);
	//void uniforms(GLint program);
private:

	GLenum textureEnums[32];
	unsigned textureCount;

	TextureFactory();
	virtual ~TextureFactory();
};

