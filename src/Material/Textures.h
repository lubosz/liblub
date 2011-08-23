/*
 * Copyright © 2010 Lubosz Sarnecki
 * Textures.h
 *
 *  Created on: Oct 4, 2010
 */

#pragma once

#include "Texture.h"

class DepthTexture : public Texture {
 public:
	DepthTexture(QSize& res, string name);
};

class ShadowTexture : public Texture {
 public:
  ShadowTexture(QSize& res, string name);
};

class ColorTexture : public Texture {
 public:
	ColorTexture(QSize& res, string name);
};

class TextureFile : public Texture {
 public:
	TextureFile(string filename, string name);
};


class TextureQImage : public Texture {
 public:
  TextureQImage(QImage * image, string name);
};


class SplatTexture : public Texture {
 public:
	SplatTexture(string name, int resolution);
	/**
	* EvalHermite(float pA, float pB, float vA, float vB, float u)
	* @brief Evaluates Hermite basis functions for the specified coefficients.
	*/
	inline float evalHermite(float pA, float pB, float vA, float vB, float u);
	unsigned char* createGaussianMap(int N);
};

class CubeTextureFile : public Texture {
 public:
	CubeTextureFile(string filename, string name);
};





