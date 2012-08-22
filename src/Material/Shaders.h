/*
 * Copyright © 2010 Lubosz Sarnecki
 * MaterialTypes.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include <string>
#include "Renderer/OpenGL.h"
#include "Material/ShaderProgram.h"

class MinimalProgram: public ShaderProgram {
 public:
    MinimalProgram() : ShaderProgram("minimal") {
    attachShader("Common/minimal.vert", GL_VERTEX_SHADER, false);
    QList<string> attributes;
    init(attributes);
  }
};

class SimpleProgram: public ShaderProgram {
 public:
  SimpleProgram(string shaders, const QList<string> & attributes) : ShaderProgram(shaders) {
    attachVertFrag(shaders, false);
    init(attributes);
    samplerUniforms();
  }
  virtual ~SimpleProgram(){

  }
};

class TemplateProgram: public ShaderProgram {
 public:
  TemplateProgram(string shaders, const QList<string> & attributes) : ShaderProgram(shaders) {
    attachVertFrag(shaders, true);
    init(attributes);
    samplerUniforms();
  }
};

class EmptyProgram: public ShaderProgram {
 public:
    EmptyProgram() : ShaderProgram("empty") {
  }
};

