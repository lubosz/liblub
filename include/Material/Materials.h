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
		shaderProgram->attachShader("Common/minimal.vert", GL_VERTEX_SHADER);
		QList<string> attributes;
		done(attributes);
  }
	void uniforms() {}
};

class DepthMaterial : public Material {
  public:
  DepthMaterial() {
    init();
    shaderProgram->attachShader("Common/depth.vert", GL_VERTEX_SHADER);
    shaderProgram->attachShader("Common/depth.frag", GL_FRAGMENT_SHADER);
    done(QList<string>());
  }
  void uniforms() {}
};

class Simple : public Material {
 public:
    Simple(string shaders, const QList<string> & attributes) {
		init();
		shaderProgram->attachVertFrag(shaders);
    done(attributes);
	}
	void uniforms() {}
};

class EmptyMat : public Material {
 public:
	EmptyMat() {}
	void uniforms() {}
};

