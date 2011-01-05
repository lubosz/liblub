/*
 * Copyright © 2010 Lubosz Sarnecki
 * MaterialTypes.h
 *
 *  Created on: Mar 25, 2010
 */

#pragma once

#include "RenderEngine.h"
#include "Material.h"
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

class BrickMaterial : public Material {
 public:
	BrickMaterial() {
		init();
		shaderProgram->attachVertFrag("Procedural/brick");
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(QVector3D(1.0, 0.3, 0.2), "brickColor");
		shaderProgram->setUniform(QVector3D(0.85, 0.86, 0.84), "mortarColor");
		shaderProgram->setUniform(QVector2D(0.30, 0.15), "brickSize");
		shaderProgram->setUniform(QVector2D(0.90, 0.85), "brickPct");
	}
};

class ShadowMapSimple : public Material {
 public:
	ShadowMapSimple(unsigned width, unsigned height) {
		init();
		addTexture(TextureFactory::Instance(). depthTexture(width, height,
                "shadowMap"));
		shaderProgram->attachVertFrag("Shadow/Simple");
		done();
  }
	void uniforms() {}
};

class ShadowMapPhong : public Material {
 public:
	ShadowMapPhong(unsigned width, unsigned height) {
		init();
		addTexture(TextureFactory::Instance().depthTexture(width, height,
                "shadowMap"));
        addTexture("Paper_Texture_by_Spiteful_Pie_Stock.jpg", "diffuseTexture");
        vector<string> flags = { "receiveShadows", "useDiffuseTexture",
                "useSpotLight" };
		shaderProgram->attachVertFrag("Color/PhongColor", flags);
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(QVector4D(0.02, 0.02, 0.02, 1.0),
                "ambientSceneColor");
        shaderProgram->setUniform(diffuseColor, "diffuseMaterialColor");
        shaderProgram->setUniform(QVector4D(0.8, 0.8, 0.8, 1.0),
                "specularMaterialColor");
		shaderProgram->setUniform(4.3, "shininess");
	}
};

class Ubershader : public Material {
 public:
	Ubershader(string diffuse, string ambient, string normal) {
		init();
#ifdef USE_FBO
        addTexture(RenderEngine::Instance().
                shadowSequence->renderPasses[0]->targetTexture);
#endif
		addTexture(diffuse, "diffuseTexture");
		addTexture(ambient, "ambientTexture");
		addTexture(normal, "normalTexture");
		vector<string> flags = {
				"receiveShadows",
				"useDiffuseTexture",
				"useSpotLight",
				"usePCF",
				"useAmbientTexture",
				"useNormalTexture"
		};
		shaderProgram->attachVertFrag("Color/PhongColor", flags);
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(QVector4D(0.1, 0.1, 0.1, 1.0), "ambientSceneColor");
		shaderProgram->setUniform(diffuseColor, "diffuseMaterialColor");
		shaderProgram->setUniform(QVector4D(0.8, 0.8, 0.8, 1.0),
                "specularMaterialColor");
		shaderProgram->setUniform(4.3, "shininess");
		shaderProgram->setUniform(1.0/1200, "yPixelOffset");
		shaderProgram->setUniform(1.0/1920, "xPixelOffset");
	}
};

class UbershaderColor : public Material {
 public:
	explicit UbershaderColor(QVector3D color) {
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

class ShadowMapPCF : public Material {
 public:
	ShadowMapPCF(unsigned width, unsigned height) {
		init();
		Texture * shadowMap =
		        TextureFactory::Instance().depthTexture(width, height, "shadowMap");
		addTexture(shadowMap);
		shaderProgram->attachVertFrag("Shadow/PCF");
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(1.0/1200, "yPixelOffset");
		shaderProgram->setUniform(1.0/1920, "xPixelOffset");
	}
};

class TextureMaterial : public Material {
 public:
    explicit TextureMaterial(string texture) {
		init();
		addTexture(texture, "myTexture");
		shaderProgram->attachVertFrag("Texture/texture");
		done();
  }
	void uniforms() {}
};

class MultiTextureMaterial : public Material {
 public:
	MultiTextureMaterial() {
		init();
		addTexture("bunny.png", "myTexture");
		addTexture("jet1.jpg", "myOtherTexture");
		shaderProgram->attachVertFrag("Texture/multitexture");
		done();
  }
	void uniforms() {}
};

class EarthMaterial : public Material {
 public:
	EarthMaterial() {
		init();
		addTexture("earth/day.jpg", "EarthDay");
		addTexture("earth/night.jpg", "EarthNight");
		addTexture("earth/clouds.jpg", "EarthCloudGloss");
		shaderProgram->attachVertFrag("Space/earth");
		done();
  }
	void uniforms() {}
};

class ProcBumpMaterial : public Material {
 public:
	ProcBumpMaterial() {
		init();
		shaderProgram->attachVertFrag("Procedural/procBump");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();
		glUniform4f(glGetUniformLocation(
		        program, "SurfaceColor"), 0.7, 0.6, 0.18, 1.0);
		glUniform1f(glGetUniformLocation(program, "BumpDensity"), 16.0);
		glUniform1f(glGetUniformLocation(program, "BumpSize"), 0.15);
		glUniform1f(glGetUniformLocation(program, "SpecularFactor"), 0.5);
	}
};

class ConvolutionMaterial : public Material {
 public:
	ConvolutionMaterial() {
		init();
		addTexture("bunny.png", "BaseImage");
		shaderProgram->attachShader("Texture/texture.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("Post/old/smoothing.frag", GL_FRAGMENT_SHADER);
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();
		// array of offsets for accessing the base image
		glUniform2f(glGetUniformLocation(program, "Offset"), 0.0, 0.0);
		// size of kernel (width * height) for this execution
		glUniform1i(glGetUniformLocation(program, "KernelSize"), 9);
		// value for each location in the convolution kernel
		glUniform4f(glGetUniformLocation(program, "ScaleFactor"), 1.0, 1.0, 1.0, 1.0);
	}
};

class SSS : public Material {
 public:
	SSS() {
		init();
		shaderProgram->attachVertFrag("Stuff/sss");
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(1.0, "SpecPower");
		shaderProgram->setUniform(1.0, "RimScalar");
		shaderProgram->setUniform(1.0, "MaterialThickness");
		shaderProgram->setUniform(
		        QVector3D(.1, .2, .3), "ExtinctionCoefficient");
		shaderProgram->setUniform(
		        QVector4D(.1, .2, .3, 1), "BaseColor");
		shaderProgram->setUniform(
		        QVector4D(.1, .2, .3, 1), "SpecColor");
	}
};

class OceanMat : public Material {
 public:
	OceanMat() {
		init();
		addTextureCube("cubemaps/morning", "EnvironmentMap");
		addTexture("ocean/waves2.dds", "NormalMap");
		shaderProgram->attachVertFrag("Water/Ocean");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();

		// Frag
		glUniform4f(glGetUniformLocation(program, "deepColor"), 0, 0.3, 0.5, 1.0);
		glUniform4f(glGetUniformLocation(program, "shallowColor"), 0, 1, 1, 1.0);
		glUniform4f(glGetUniformLocation(
		        program, "reflectionColor"), 0.95, 1, 1, 1.0);

		glUniform1f(glGetUniformLocation(program, "reflectionAmount"), 1.0);
		glUniform1f(glGetUniformLocation(program, "reflectionBlur"), 0.0);
		glUniform1f(glGetUniformLocation(program, "waterAmount"), 0.3);
		glUniform1f(glGetUniformLocation(program, "fresnelPower"), 5.0);
		glUniform1f(glGetUniformLocation(program, "fresnelBias"), 0.328);
		glUniform1f(glGetUniformLocation(program, "hdrMultiplier"), 0.471);

		// vert
		glUniform2f(glGetUniformLocation(program, "textureScale"), 25, 26);
		glUniform2f(glGetUniformLocation(program, "bumpSpeed"), 0.015, 0.005);

		// param_named_auto eyePosition camera_position_object_space
		glUniform3f(glGetUniformLocation(program, "eyePosition"), 0.5, 0.5, 0.5);

		glUniform1f(glGetUniformLocation(program, "BumpScale"), 0.2);
		glUniform1f(glGetUniformLocation(program, "time"), 0.7);
		glUniform1f(glGetUniformLocation(program, "waveFreq"), 0.028);
		glUniform1f(glGetUniformLocation(program, "waveAmp"), 1.8);
	}
};

class ParticleMat : public Material {
 public:
	ParticleMat() {
		init();
		addTexture(TextureFactory::Instance().splatTexture("splatTexture", 32));
		shaderProgram->attachVertFrag("Particle/particle");
		done();
  }
	void uniforms() {}
};

class CubeMap : public Material {
 public:
    explicit CubeMap(string texture) {
		init();
		addTextureCube(texture, "EnvMap");
		shaderProgram->attachVertFrag("Texture/cubemap");
		done();
  }
	void uniforms() {
		shaderProgram->setUniform(QVector3D(0.5, 0.5, 1.0), "BaseColor");
		shaderProgram->setUniform(0.9, "MixRatio");
	}
};

class RemeshMat : public Material {
 public:
	RemeshMat() {
		init();
		addTexture("remesh/multi.tga", "terrain_texture");
		addTexture("remesh/multi.tga", "displacement_texture");
		shaderProgram->attachVertFragGeom("Remesh/render2");
		done();
  }
	void uniforms() {}
};

class PhongColorMat : public Material {
 public:
	explicit PhongColorMat(QVector3D color) {
		init();
		shaderProgram->attachVertFrag("Color/PhongColor");
		diffuseColor = color.toVector4D();
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();

		// ambient
		glUniform4f(glGetUniformLocation(
		        program, "ambientSceneColor"), 0.0, 0.0, 0.0, 1.0);

		// diffuse
		shaderProgram->setUniform(diffuseColor, "diffuseMaterialColor");

		// specular
		glUniform4f(glGetUniformLocation(
		        program, "specularMaterialColor"), 0.8, 0.8, 0.8, 1.0);
		glUniform1f(glGetUniformLocation(program, "shininess"), 4.3);
	}
};

class PhongTexMat : public Material {
 public:
	explicit PhongTexMat(string texture) {
		init();
		addTexture(texture, "diffuseTexture");
		vector<string> flags = {"useDiffuseTexture"};
		shaderProgram->attachVertFrag("Color/PhongColor", flags);
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();

		// ambient
		glUniform4f(glGetUniformLocation(
		        program, "lightColor"), 0.8, 0.8, 0.8, 1.0);
		glUniform4f(glGetUniformLocation(
		        program, "ambientSceneColor"), 0.0, 0.0, 0.0, 1.0);

		// specular
		glUniform4f(glGetUniformLocation(
		        program, "specularMaterialColor"), 0.5, 0.5, 0.5, 1.0);
		glUniform1f(glGetUniformLocation(program, "shininess"), 4.3);
	}
};

class NormalMapMaterial : public Material {
 public:
	NormalMapMaterial(string diffuse, string normal) {
		init();
		addTexture(diffuse, "colorMap");
		addTexture(normal, "normalMap");
		shaderProgram->attachVertFrag("Bump/normalMap");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();

		// ambient
		glUniform4f(glGetUniformLocation(
		        program, "lightColor"), 0.8, 0.8, 0.8, 1.0);
		glUniform4f(glGetUniformLocation(
		        program, "ambientSceneColor"), 0.0, 0.0, 0.0, 1.0);

		glUniform1f(glGetUniformLocation(
		        program, "invRadius"), 0.9);
		shaderProgram->setUniform(QVector4D(1, 1, 1, 1), "diffuseMaterialColor");

		// specular
		glUniform4f(glGetUniformLocation(
		        program, "specularMaterialColor"), 0.5, 0.5, 0.5, 1.0);
		glUniform1f(glGetUniformLocation(
		        program, "shininess"), 4.3);
	}
};

class SphereMap : public Material {
 public:
	SphereMap(string diffuse, string env) {
		init();
		addTexture(diffuse, "colorMap");
		addTexture(env, "envMap");
		shaderProgram->attachVertFrag("Env/sphereMap");
		done();
  }
	void uniforms() {}
};

class MandelMat : public Material {
 public:
	MandelMat() {
		init();
		shaderProgram->attachVertFrag("Procedural/mandelbrot");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();

		glUniform1f(glGetUniformLocation(program, "SpecularContribution"), 1.0);
		glUniform1f(glGetUniformLocation(program, "DiffuseContribution"), 1.0);
		glUniform1f(glGetUniformLocation(program, "Shininess"), 1.0);
		glUniform1f(glGetUniformLocation(program, "MaxIterations"), 100.0);
		glUniform1f(glGetUniformLocation(program, "Zoom"), 1.0);
		glUniform1f(glGetUniformLocation(program, "Xcenter"), 1.0);
		glUniform1f(glGetUniformLocation(program, "Ycenter"), 1.0);

		glUniform3f(glGetUniformLocation(program, "InnerColor"), 1.0, 0.0, 0.0);
		glUniform3f(glGetUniformLocation(program, "OuterColor1"), 0.0, 1.0, 0.0);
		glUniform3f(glGetUniformLocation(program, "OuterColor2"), 0.0, 0.0, 1.0);
	}
};


class HatchingMat : public Material {
 public:
	HatchingMat() {
		init();
		shaderProgram->attachVertFrag("Procedural/hatching");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();
		glUniform1f(glGetUniformLocation(program, "Time"), 1.0);
	}
};

class GeomTest : public Material {
 public:
	GeomTest() {
		init();
		shaderProgram->attachVertFragGeom("Geometry/test");
		done();
  }
	void uniforms() {}
};

class FogMaterial : public Material {
 public:
	FogMaterial() {
		init();
		shaderProgram->attachVertFrag("Stuff/Fog");
		done();
  }
	void uniforms() {
		GLuint program = shaderProgram->getReference();

		glUniform1f(glGetUniformLocation(program, "uFogPos"), 0.8);
		glUniform3f(glGetUniformLocation(program, "uEyePos"), 0.0, 0.0, 1.0);
	}
};
