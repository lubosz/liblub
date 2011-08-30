/*
 * Copyright © 2010 Lubosz Sarnecki
 * Material.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include <vector>
#include <string>
#include "Material/ShaderProgram.h"
#include "Material/Texture.h"

class Material {
 public:
  virtual ~Material() {
  }
  ShaderProgram *getShaderProgram();
  void activate();
  void samplerUniforms();
  void addTexture(Texture * texture);
  void addTextures(vector<Texture *> &addTextures);
  void done(const QList<string> & attributes);

  vector<Texture*> textures;

  ShaderProgram * shaderProgram;
  void init();
  void initRenderTargets(vector<Texture*> &targets);
  void activateTextures();
 protected:
  QVector4D diffuseColor;

  void addTexture(string file, string name);
  void addTextureCube(string file, string name);

 private:
  virtual void uniforms() = 0;
  void defaultAttribs();
};





