/*
 * Copyright © 2010 Lubosz Sarnecki
 * MaterialTypes.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include <string>
#include "Renderer/RenderEngine.h"
#include "Material/Material.h"
class RenderEngine;

class Minimal: public Material {
 public:
  Minimal() {
    init();
    shaderProgram->attachShader("Common/minimal.vert", GL_VERTEX_SHADER, false);
    QList<string> attributes;
    done(attributes);
  }
  void uniforms() {
  }
};

class Simple: public Material {
 public:
  Simple(string shaders, const QList<string> & attributes) {
    init();
    shaderProgram->attachVertFrag(shaders, false);
    done(attributes);
  }
  void uniforms() {
  }
};

class Template: public Material {
 public:
  Template(string shaders, const QList<string> & attributes) {
    init();
    shaderProgram->attachVertFrag(shaders, true);
    done(attributes);
  }
  void uniforms() {
  }
};

class EmptyMat: public Material {
 public:
  EmptyMat() {
  }
  void uniforms() {
  }
};

