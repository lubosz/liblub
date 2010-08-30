/*
 * Texture.h
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"

#include <string>
#include <FreeImagePlus.h>



class Texture {
public:
	Texture(GLenum glId, string name, int resolution);
	Texture(string filename, GLenum glId, string name, GLuint textureID);
	Texture(string filename, GLenum glId, string name, bool cube);

	void activate();
	void bind();
	void uniform(GLuint program);
private:
	GLenum glId;
	string name;
	GLuint texture;
	GLuint textureType;
	GLuint textureID;

	virtual ~Texture();

	fipImage * readImage(string path, GLint * glChannelOrder, GLint * texChannelOrder);
	unsigned char* createGaussianMap(int N);
	inline float evalHermite(float pA, float pB, float vA, float vB, float u);
};
