/*
 * Copyright © 2010 Lubosz Sarnecki
 * MaterialTypes.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include <string>
#include "Renderer/RenderEngine.h"
#include "Material/ShaderProgram.h"

class MinimalProgram: public ShaderProgram {
 public:
  MinimalProgram() {
    attachShader("Common/minimal.vert", GL_VERTEX_SHADER, false);
    QList<string> attributes;
    init(attributes);
  }
};

class SimpleProgram: public ShaderProgram {
 public:
  SimpleProgram(string shaders, const QList<string> & attributes) {
    attachVertFrag(shaders, false);
    init(attributes);
    samplerUniforms();
  }
};

class TemplateProgram: public ShaderProgram {
 public:
  TemplateProgram(string shaders, const QList<string> & attributes) {
    attachVertFrag(shaders, true);
    init(attributes);
    samplerUniforms();
  }
};

class EmptyProgram: public ShaderProgram {
 public:
  EmptyProgram() {
  }
};

