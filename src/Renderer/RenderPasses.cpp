/*
 * RenderPasses.cpp
 *
 *  Created on: Oct 5, 2010
 *      Author: bmonkey
 */

#include "RenderPasses.h"
#include "Materials.h"
#include "MediaLayer.h"
#include "Camera.h"

LightViewDepthPass::LightViewDepthPass(FrameBuffer * fbo){
	this->fbo = fbo;
	targetTexture = TextureFactory::Instance().depthTexture(fbo->width, fbo->height, "shadowMap");
	fbo->attachTexture(GL_DEPTH_ATTACHMENT, targetTexture);
	fbo->disableColorBuffer();

	material = new Minimal();
}

void LightViewDepthPass::prepare() {
	fbo->bind();

	RenderEngine::Instance().clear();

	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	fbo->updateRenderView();

	// Clear previous frame values
	//glClear( GL_DEPTH_BUFFER_BIT);

	//Disable color rendering, we only want to write to the Z-Buffer
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	//glCullFace(GL_FRONT);

	glPolygonOffset(2.0, 0.0);
	//glPolygonOffset(1.1, 4.0);
	material->activate();
}

void LightViewDepthPass::draw(){
	SceneGraph::Instance().drawCasters(material);
}

void LightViewDepthPass::cleanUp(){
	fbo->unBind();
	glPolygonOffset(0.0,0.0);
	//glCullFace(GL_BACK);
	//Enabling color write (previously disabled for light POV z-buffer rendering)
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

LightTogglePass::LightTogglePass(){
}

void LightTogglePass::prepare() {
	RenderEngine::Instance().clear();
}

void LightTogglePass::draw(){
	glViewport(0,0,MediaLayer::Instance().width, MediaLayer::Instance().height);
	if(RenderEngine::Instance().lightView){
		SceneGraph::Instance().drawNodes(SceneGraph::Instance().light);
	}else{
		SceneGraph::Instance().drawNodes(&Camera::Instance());
	}
}

void LightTogglePass::cleanUp(){}
