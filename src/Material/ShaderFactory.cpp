/*
 * ShaderFactory.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include <FreeImagePlus.h>
#include "ShaderFactory.h"
#include "Texture.h"
#include "RenderEngine.h"
#include "TextureFactory.h"
#include "Material.h"
#include "MaterialTypes.h"

ShaderFactory::ShaderFactory() {
	// TODO Auto-generated constructor stub
}

ShaderFactory::~ShaderFactory() {
	// TODO Auto-generated destructor stub
}

void ShaderFactory::vertexColorShader(){
	VertexMaterial(RenderEngine::Instance().shaderProgram);
}

void ShaderFactory::brickShader(){
	materials.push_back(new BrickMaterial(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::textureShader(){
	TextureMaterial(RenderEngine::Instance().shaderProgram);
}

void ShaderFactory::multiTextureShader(){
	MultiTextureMaterial(RenderEngine::Instance().shaderProgram);
}

void ShaderFactory::earthShader(){
	materials.push_back(new EarthMaterial(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::coneMapShader(){
	materials.push_back(new ConeMapMaterial(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::procBump(){
	materials.push_back(new ProcBumpMaterial(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::convolution(){
	materials.push_back(new ConvolutionMaterial(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::bump1(){
	materials.push_back(new BumpMaterial1(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::bump2(){
	materials.push_back(new BumpMaterial2(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::relief(){
	materials.push_back(new ReliefMat(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::ocean(){
	materials.push_back(new OceanMat(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::particle(){
	materials.push_back(new ParticleMat(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::env(){
	materials.push_back(new EnvMat(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::normalcolor(){
	materials.push_back(new NormalColorMat(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::stars(){
	materials.push_back(new StarMat(RenderEngine::Instance().shaderProgram));
}

void ShaderFactory::addUniforms(){
	GLint program = RenderEngine::Instance().shaderProgram->program;

	TextureFactory::Instance().uniforms(program);

	cout << "Material size: " << materials.size() << "\n";
	for (int i = 0; i < materials.size(); i++){
		cout << "Doing material #" << i << "\n";
		materials[i]->uniforms();
	}
}
