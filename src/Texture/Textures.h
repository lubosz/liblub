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
  TextureQImage(const QImage &image, string name);
};

class CubeTextureFile : public Texture {
 public:
	CubeTextureFile(string filename, string name);
};

class Texture3D : public Texture {
 public:
    Texture3D(string name, GLubyte *voxels, unsigned size);
};
