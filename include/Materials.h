/*
 * MaterialTypes.h
 *
 *  Created on: Mar 25, 2010
 *      Author: bmonkey
 */

#pragma once

#include "ShaderProgram.h"
#include "Material.h"
#include "TextureFactory.h"
#include "RenderEngine.h"

class VertexColorMaterial : public Material {
public:
	VertexColorMaterial(){
		init();
		attachVertFragGeom("Color/vertexcolor");
		done();
  }
	void uniforms(){}
};

class BrickMaterial : public Material {
public:
	BrickMaterial(){
		init();
		attachVertFrag("Procedural/brick");
		done();
  }
	void uniforms(){
		cout << "inititalizing brick uniforms.\n";
		glUniform3f(glGetUniformLocation(shaderProgram->getReference(), "BrickColor"), 1.0, 0.3, 0.2);
		glUniform3f(glGetUniformLocation(shaderProgram->getReference(), "MortarColor"), 0.85, 0.86, 0.84);
		glUniform2f(glGetUniformLocation(shaderProgram->getReference(), "BrickSize"), 0.30, 0.15);
		glUniform2f(glGetUniformLocation(shaderProgram->getReference(), "BrickPct"), 0.90, 0.85);
	}
};

class TextureMaterial : public Material {
public:
	TextureMaterial(string texture){
		init();
		addTexture(texture,"myTexture");
		attachVertFrag("Texture/texture");
		done();

  }
	void uniforms(){}

};

class FBOMaterial : public Material {
public:
	FBOMaterial(){
		init();
		attachVertFrag("Texture/FBOtexture");
		//done();

  }
	void uniforms(){}

};

class MultiTextureMaterial : public Material {
public:
	MultiTextureMaterial(){
		init();
		addTexture("bunny.png","myTexture");
		addTexture("jet1.jpg","myOtherTexture");
		attachVertFrag("Texture/multitexture");
		done();
  }
	void uniforms(){}
};

class EarthMaterial : public Material {
public:
	EarthMaterial(){
		init();
		addTexture("earth/day.jpg","EarthDay");
		addTexture("earth/night.jpg","EarthNight");
		addTexture("earth/clouds.jpg","EarthCloudGloss");
		attachVertFrag("Space/earth");
		done();
  }
	void uniforms(){}
};

class ConeMapMaterial : public Material {
public:
	ConeMapMaterial(){
		init();
		addTexture("cone/collage_base.jpg","texmap");
		addTexture("cone/collage_step.png","stepmap");
		shaderProgram->attachShader("Common/shared.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("CSM/csm_dist_shadow.frag", GL_FRAGMENT_SHADER);
        done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();
		glUniform4f(glGetUniformLocation(program, "ambient"), 0.2, 0.2, 0.2, 1.0);
		glUniform4f(glGetUniformLocation(program, "diffuse"), 0.99, 0.99, 0.99, 1.0);

		glUniform1f(glGetUniformLocation(program, "depth"), 0.05);

		signed int texture_size = 512;
		glUniform1f(glGetUniformLocation(program, "texsize"), static_cast<float>(texture_size));
		glUniform1f(glGetUniformLocation(program, "csm_gain"), 1.0);
		glUniform1f(glGetUniformLocation(program, "csm_offset"), 0.0);
		glUniform1f(glGetUniformLocation(program, "linearAttenuation"), 1.0);

		glUniform1i(glGetUniformLocation(program, "conesteps"), 20);
	}
};

class ProcBumpMaterial : public Material {
public:
	ProcBumpMaterial(){
		init();
		attachVertFrag("Procedural/procBump");
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();
		glUniform4f(glGetUniformLocation(program, "SurfaceColor"), 0.7, 0.6, 0.18, 1.0);
		glUniform1f(glGetUniformLocation(program, "BumpDensity"), 16.0);
		glUniform1f(glGetUniformLocation(program, "BumpSize"), 0.15);
		glUniform1f(glGetUniformLocation(program, "SpecularFactor"), 0.5);
	}
};

class ConvolutionMaterial : public Material {
public:
	ConvolutionMaterial(){
		init();
		addTexture("bunny.png","BaseImage");
		shaderProgram->attachShader("Texture/texture.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("Post/smoothing.frag", GL_FRAGMENT_SHADER);
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();
		// array of offsets for accessing the base image
		glUniform2f(glGetUniformLocation(program, "Offset"), 0.0, 0.0);
		// size of kernel (width * height) for this execution
		glUniform1i(glGetUniformLocation(program, "KernelSize"), 9);
		// value for each location in the convolution kernel
		//glUniform4f(glGetUniformLocation(program, "KernelValue"),0,1,0,1,-4,1,0,1,0);
		glUniform4f(glGetUniformLocation(program, "ScaleFactor"), 1.0, 1.0, 1.0, 1.0);
	}
};


class BumpMaterial : public Material {
public:
	BumpMaterial(string name){
		init();
		addTexture(name+".jpg","diffuseTexture");
		addTexture(name+"-normal.png","normalTexture");
		attachVertFrag("Bump/noTangentBump");
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();

		glUniform1i(glGetUniformLocation(program, "Mode"), 1);

	}
};

class PlanetMat : public Material {
public:
	PlanetMat(string name){
		init();
		addTexture(name+".jpg","diffuseTexture");
		addTexture(name+"-normal.png","normalTexture");
		attachVertFrag("Space/Planet");
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();

		glUniform1i(glGetUniformLocation(program, "Mode"), 1);

	}
};

class BumpMaterial2 : public Material {
public:
	BumpMaterial2(){
		init();
		addTexture("bunny.png","colorMap");
		addTexture("bunny-bump.png","normalMap");
		addTexture("bunny-gloss.png","glossMap");
		attachVertFrag("Bump/noTangentBump2");
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();


		glUniform1f(glGetUniformLocation(program, "glossMax"), 1.0);
		glUniform1f(glGetUniformLocation(program, "glossNoise"), 1.0);


		glUniform4f(glGetUniformLocation(program, "glossColor"), 1.0, 1.0, 1.0, 1.0);

		glUniform4f(glGetUniformLocation(program, "LightAmbient"), 1.0, 1.0, 1.0, 1.0);
		glUniform4f(glGetUniformLocation(program, "LightDiffuse"), 1.0, 1.0, 1.0, 1.0);
		glUniform4f(glGetUniformLocation(program, "LightSpecular"), 1.0, 1.0, 1.0, 1.0);
		glUniform4f(glGetUniformLocation(program, "MaterialDiffuse"), 0.9, 0.8, 1.0, 1.0);
		glUniform4f(glGetUniformLocation(program, "MaterialSpecular"), 1.0, 1.0, 1.0, 1.0);

	}
};

class ReliefMat : public Material {
public:
	ReliefMat(){
		init();
		addTexture("cone/collage_base.jpg","texmap");
		addTexture("cone/collage_step.png","reliefmap");
		shaderProgram->attachShader("Common/shared.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("Bump/relief.frag", GL_FRAGMENT_SHADER);
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();

		glUniform1i(glGetUniformLocation(program, "linear_search_steps"), 1);
		glUniform1i(glGetUniformLocation(program, "binary_search_steps"), 1);

		glUniform1f(glGetUniformLocation(program, "shine"), 1.0);
		glUniform1f(glGetUniformLocation(program, "depth"), 1.0);
		glUniform1f(glGetUniformLocation(program, "tile"), 1.0);
		glUniform1f(glGetUniformLocation(program, "texsize"), 1.0);
		glUniform1f(glGetUniformLocation(program, "linearAttenuation"), 1.0);


		glUniform4f(glGetUniformLocation(program, "ambient"), 1.0, 1.0, 1.0, 1.0);
		glUniform4f(glGetUniformLocation(program, "specular"), 1.0, 1.0, 1.0, 1.0);
		glUniform4f(glGetUniformLocation(program, "diffuse"), 0.9, 0.8, 1.0, 1.0);

	}
};

class OceanMat : public Material {
public:
	OceanMat(){
		init();
		addTextureCube("cubemaps/morning","EnvironmentMap");
		addTexture("ocean/waves2.dds","NormalMap");
		attachVertFrag("Water/Ocean");
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();

		//Frag
		glUniform4f(glGetUniformLocation(program, "deepColor"), 0, 0.3, 0.5, 1.0);
		glUniform4f(glGetUniformLocation(program, "shallowColor"), 0, 1, 1, 1.0);
		glUniform4f(glGetUniformLocation(program, "reflectionColor"), 0.95, 1, 1, 1.0);

		glUniform1f(glGetUniformLocation(program, "reflectionAmount"), 1.0);
		glUniform1f(glGetUniformLocation(program, "reflectionBlur"), 0.0);
		glUniform1f(glGetUniformLocation(program, "waterAmount"), 0.3);
		glUniform1f(glGetUniformLocation(program, "fresnelPower"), 5.0);
		glUniform1f(glGetUniformLocation(program, "fresnelBias"), 0.328);
		glUniform1f(glGetUniformLocation(program, "hdrMultiplier"), 0.471);

		//vert
		glUniform2f(glGetUniformLocation(program, "textureScale"), 25, 26);
		glUniform2f(glGetUniformLocation(program, "bumpSpeed"), 0.015, 0.005);

		//param_named_auto eyePosition camera_position_object_space
		glUniform3f(glGetUniformLocation(program, "eyePosition"), 0.5, 0.5, 0.5);

		glUniform1f(glGetUniformLocation(program, "BumpScale"), 0.2);
		glUniform1f(glGetUniformLocation(program, "time"), 0.7);
		glUniform1f(glGetUniformLocation(program, "waveFreq"), 0.028);
		glUniform1f(glGetUniformLocation(program, "waveAmp"), 1.8);

	}
};

class ParticleMat : public Material {
public:
	ParticleMat(){
		init();
		addTexture(TextureFactory::Instance().splatTexture("splatTexture", 32));
		attachVertFrag("Particle/particle");
		done();
  }
	void uniforms(){


	}
};

class EnvMat : public Material {
public:
	EnvMat(){
		init();
		addTextureCube("cubemaps/sky","EnvMap");
		attachVertFrag("Texture/cubemap");
		done();
		glError("MatTypes",280);
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();

		//Frag
		glUniform3f(glGetUniformLocation(program, "BaseColor"), 0.5, 0.5, 1.0);
		glUniform1f(glGetUniformLocation(program, "MixRatio"), 0.9);

	}
};

class NormalColorMat : public Material {
public:
	NormalColorMat(){
		init();
		attachVertFrag("Color/normalcolor");
		done();
  }
	void uniforms(){}
};

class UvColorMat : public Material {
public:
	UvColorMat(){
		init();
		attachVertFrag("Color/uvcolor");
		done();
  }
	void uniforms(){}
};

class StarMat : public Material {
public:
	StarMat(){
		init();
		attachVertFrag("Particle/stars");
		done();
  }
	void uniforms(){}
};

class BigEarthMat : public Material {
public:
	BigEarthMat(){
		init();
		addTexture("Earth/EarthMap_2500x1250.jpg","myTexture");
		attachVertFrag("Texture/texture");
		done();

  }
	void uniforms(){}

};


class MarsMat : public Material {
public:
	MarsMat(){
		init();
		addTexture("Earth/MarsMap_2500x1250.jpg","myTexture");
		attachVertFrag("Texture/texture");
		done();

  }
	void uniforms(){}

};

class RemeshMat : public Material {
public:
	RemeshMat(){
		init();
		//addTexture("remesh/shaderX7.tga","terrain_texture");
		//addTexture("remesh/shaderX7-disp.tga","displacement_texture");
		addTexture("remesh/multi.tga","terrain_texture");
		addTexture("remesh/multi.tga","displacement_texture");
		//attachVertGeom("Remesh/render");
		attachVertFragGeom("Remesh/render2");
		done();

  }
	void uniforms(){}

};

class PhongTextureMat : public Material {
public:
	PhongTextureMat(){
		init();
		addTexture("Earth/MarsMap_2500x1250.jpg","myTexture");
		attachVertFrag("Texture/PhongTexture");
		done();

  }
	void uniforms(){}

};

class PhongColorMat : public Material {
public:
	PhongColorMat(){
		init();
		attachVertFrag("Color/PhongColor");
		done();

  }
	void uniforms(){}

};

class WhiteMat : public Material {
public:
	WhiteMat(){
		init();
		attachVertFrag("Color/white");
		done();

  }
	void uniforms(){}

};

class MandelMat : public Material {
public:
	MandelMat(){
		init();
		attachVertFrag("Procedural/mandelbrot");
		done();
  }
	void uniforms(){
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
	HatchingMat(){
		init();
		attachVertFrag("Procedural/hatching");
		done();
  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();
		glUniform1f(glGetUniformLocation(program, "Time"), 1.0);
	}
};

class GeomTest : public Material {
public:
	GeomTest(){
		init();
		attachVertFragGeom("Geometry/test");
		done();

  }
	void uniforms(){}

};

class PhongTextureMat2 : public Material {
public:
	PhongTextureMat2(){
		init();
		addTexture("Paper_Texture_by_Spiteful_Pie_Stock.jpg","texture");
		attachVertFrag("Texture/PhongTexture2");
		done();

  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();

		glUniform1f(glGetUniformLocation(program, "shininess"), 0.8);
		glUniform4f(glGetUniformLocation(program, "specular"), 0.9, 0.9, 0.9, 1.0);
	}
};

class FogMaterial : public Material {
public:
	FogMaterial(){
		init();
		attachVertFrag("Stuff/Fog");
		done();

  }
	void uniforms(){
		GLuint program = shaderProgram->getReference();

		glUniform1f(glGetUniformLocation(program, "uFogPos"), 0.8);
		glUniform3f(glGetUniformLocation(program, "uEyePos"), 0.0, 0.0, 1.0);
	}
};