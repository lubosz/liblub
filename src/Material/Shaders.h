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
    attachShader("Common/minimal.vert", GL_VERTEX_SHADER);
    QList<string> attributes;
    init(attributes);
  }
};

class VertFragProgram: public ShaderProgram {
 public:
  VertFragProgram(string shaders, const QList<string> & attributes) : ShaderProgram(shaders) {
    attachVertFrag(shaders);
    init(attributes);
    samplerUniforms();
  }
};
