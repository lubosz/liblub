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

class VertexMaterial : public Material {
public:
	VertexMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;
		shaderProgram->attachShader("tutorial4.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("tutorial4.geom", GL_GEOMETRY_SHADER);
		shaderProgram->attachShader("tutorial4.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){}
};

class BrickMaterial : public Material {
public:
	BrickMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;
		shaderProgram->attachShader("brick.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("brick.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		cout << "inititalizing brick uniforms.\n";
		glUniform3f(glGetUniformLocation(shaderProgram->program, "BrickColor"), 1.0, 0.3, 0.2);
		glUniform3f(glGetUniformLocation(shaderProgram->program, "MortarColor"), 0.85, 0.86, 0.84);
		glUniform2f(glGetUniformLocation(shaderProgram->program, "BrickSize"), 0.30, 0.15);
		glUniform2f(glGetUniformLocation(shaderProgram->program, "BrickPct"), 0.90, 0.85);
	}
};

class TextureMaterial : public Material {
public:
	TextureMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;
		TextureFactory::Instance().load("bunny.png","myTexture");

		shaderProgram->attachShader("texture.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("texture.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){}
};

class MultiTextureMaterial : public Material {
public:
	MultiTextureMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;
		TextureFactory::Instance().load("bunny.png","myTexture");
		TextureFactory::Instance().load("jet1.jpg","myOtherTexture");

		shaderProgram->attachShader("multitexture.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("multitexture.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){}
};

class EarthMaterial : public Material {
public:
	EarthMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;
		TextureFactory::Instance().load("earth/day.jpg","EarthDay");
		TextureFactory::Instance().load("earth/night.jpg","EarthNight");
		TextureFactory::Instance().load("earth/clouds.jpg","EarthCloudGloss");

		shaderProgram->attachShader("earth.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("earth.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){}
};

class ConeMapMaterial : public Material {
public:
	ConeMapMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;
		TextureFactory::Instance().load("cone/collage_base.jpg","texmap");
		TextureFactory::Instance().load("cone/collage_step.png","stepmap");

		shaderProgram->attachShader("shared.vert", GL_VERTEX_SHADER);
                shaderProgram->attachShader("csm_dist_shadow.frag", GL_FRAGMENT_SHADER);
                //shaderProgram->attachShader("procBump.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		GLuint program = shaderProgram->program;
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
	ProcBumpMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;
		shaderProgram->attachShader("procBump.vert", GL_VERTEX_SHADER);
		shaderProgram->attachShader("procBump.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		GLuint program = shaderProgram->program;
		glUniform4f(glGetUniformLocation(program, "SurfaceColor"), 0.7, 0.6, 0.18, 1.0);
		glUniform1f(glGetUniformLocation(program, "BumpDensity"), 16.0);
		glUniform1f(glGetUniformLocation(program, "BumpSize"), 0.15);
		glUniform1f(glGetUniformLocation(program, "SpecularFactor"), 0.5);
	}
};

class ConvolutionMaterial : public Material {
public:
	ConvolutionMaterial(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;

		TextureFactory::Instance().load("bunny.png","BaseImage");
		RenderEngine::Instance().shaderProgram->attachShader("texture.vert", GL_VERTEX_SHADER);
		//RenderEngine::Instance().shaderProgram->attachShader("convolution.frag", GL_FRAGMENT_SHADER);
		RenderEngine::Instance().shaderProgram->attachShader("smoothing.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		GLuint program = shaderProgram->program;
		// array of offsets for accessing the base image
		glUniform2f(glGetUniformLocation(program, "Offset"), 0.0, 0.0);
		// size of kernel (width * height) for this execution
		glUniform1i(glGetUniformLocation(program, "KernelSize"), 9);
		// value for each location in the convolution kernel
		//glUniform4f(glGetUniformLocation(program, "KernelValue"),0,1,0,1,-4,1,0,1,0);
		glUniform4f(glGetUniformLocation(program, "ScaleFactor"), 1.0, 1.0, 1.0, 1.0);
	}
};


class BumpMaterial1 : public Material {
public:
	BumpMaterial1(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;

		TextureFactory::Instance().load("bunny.png","diffuseTexture");
		TextureFactory::Instance().load("bunny-bump.png","normalTexture");
		RenderEngine::Instance().shaderProgram->attachShader("noTangentBump.vert", GL_VERTEX_SHADER);
		RenderEngine::Instance().shaderProgram->attachShader("noTangentBump.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		GLuint program = shaderProgram->program;

		glUniform1i(glGetUniformLocation(program, "Mode"), 1);

	}
};

class BumpMaterial2 : public Material {
public:
	BumpMaterial2(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;

		TextureFactory::Instance().load("bunny.png","colorMap");
		TextureFactory::Instance().load("bunny-bump.png","normalMap");
		TextureFactory::Instance().load("bunny-gloss.png","glossMap");
		RenderEngine::Instance().shaderProgram->attachShader("noTangentBump2.vert", GL_VERTEX_SHADER);
		RenderEngine::Instance().shaderProgram->attachShader("noTangentBump2.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		GLuint program = shaderProgram->program;


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
	ReliefMat(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;

		TextureFactory::Instance().load("cone/collage_base.jpg","texmap");
		TextureFactory::Instance().load("cone/collage_step.png","reliefmap");
		RenderEngine::Instance().shaderProgram->attachShader("shared.vert", GL_VERTEX_SHADER);
		RenderEngine::Instance().shaderProgram->attachShader("relief.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		GLuint program = shaderProgram->program;

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
	OceanMat(ShaderProgram * shaderProgram){
		this->shaderProgram = shaderProgram;

		TextureFactory::Instance().loadCubeMap("cubemaps/morning","EnvironmentMap");
		TextureFactory::Instance().load("ocean/waves2.dds","NormalMap");
		RenderEngine::Instance().shaderProgram->attachShader("Ocean.vert", GL_VERTEX_SHADER);
		RenderEngine::Instance().shaderProgram->attachShader("Ocean.frag", GL_FRAGMENT_SHADER);

		defaultAttribs();
  }
	void uniforms(){
		GLuint program = shaderProgram->program;

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
