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
		done();
  }
	void uniforms() {}
};

class DepthMaterial : public Material {
  public:
  DepthMaterial() {
    init();
    shaderProgram->attachShader("Common/depth.vert", GL_VERTEX_SHADER);
    shaderProgram->attachShader("Common/depth.frag", GL_FRAGMENT_SHADER);
    done();
  }
  void uniforms() {}
};

class Simple : public Material {
 public:
    explicit Simple(string shaders) {
		init();
		shaderProgram->attachVertFrag(shaders);
		done();
	}
	void uniforms() {}
};

class EmptyMat : public Material {
 public:
	EmptyMat() {}
	void uniforms() {}
};

class PhongColor : public Material {
 public:
	explicit PhongColor(QVector3D color) {
		init();
#ifdef USE_FBO
		addTexture(RenderEngine::Instance().
		        shadowSequence->renderPasses[0]->targetTexture);
#endif
		vector<string> flags = {
				"receiveShadows",
				"useSpotLight",
				"usePCF"
		};
		diffuseColor = color.toVector4D();
		shaderProgram->attachVertFrag("Color/PhongColor", flags);
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(QVector4D(0.1, 0.1, 0.1, 1.0), "ambientSceneColor");
		shaderProgram->setUniform(diffuseColor, "diffuseMaterialColor");
		shaderProgram->setUniform(
		        QVector4D(0.8, 0.8, 0.8, 1.0), "specularMaterialColor");
		shaderProgram->setUniform(4.3, "shininess");
		shaderProgram->setUniform(1.0/1200, "yPixelOffset");
		shaderProgram->setUniform(1.0/1920, "xPixelOffset");
	}
};
