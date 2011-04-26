/*
 * Copyright © 2010 Lubosz Sarnecki
 * MaterialTypes.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include "Renderer/RenderEngine.h"
#include "Material/Material.h"
class RenderEngine;

class Minimal : public Material {
  public:
	Minimal() {
		init();
		shaderProgram->attachShader("Common/minimal.vert", GL_VERTEX_SHADER, false);
		QList<string> attributes;
		done(attributes);
  }
	void uniforms() {}
};

class DepthMaterial : public Material {
  public:
  DepthMaterial() {
    init();
    shaderProgram->attachShader("Common/depth.vert", GL_VERTEX_SHADER, false);
    shaderProgram->attachShader("Common/depth.frag", GL_FRAGMENT_SHADER, false);
    done(QList<string>());
  }
  void uniforms() {}
};

class Simple : public Material {
 public:
    Simple(string shaders, const QList<string> & attributes) {
		init();
		shaderProgram->attachVertFrag(shaders, false);
    done(attributes);
	}
	void uniforms() {}
};

class Template : public Material {
 public:
  Template(string shaders, const QList<string> & attributes) {
    init();
    shaderProgram->attachVertFrag(shaders, true);
    done(attributes);
  }
  void uniforms() {}
};

class EmptyMat : public Material {
 public:
	EmptyMat() {}
	void uniforms() {}
};

