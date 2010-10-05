/*
 * RenderSequence.cpp
 *
 *  Created on: Oct 4, 2010
 *      Author: bmonkey
 */

#include "RenderSequence.h"

RenderSequence::RenderSequence() {
    fbo = new FrameBuffer(1920,1200);

	pass1Mat = new ShadowMapPCF(1920, 1200);
	//pass1Mat = new FBOMaterial(width, height);
	pass2Mat = new ShadowMapDepth();

	//fbo->attachTexture(GL_COLOR_ATTACHMENT0, fbo->getDebugTexture());
	//fbo->attachTexture(GL_COLOR_ATTACHMENT0, pass1Mat->textures[0]);
	fbo->attachTexture(GL_DEPTH_ATTACHMENT, pass1Mat->textures[0]);
	fbo->disableColorBuffer();
	fbo->checkAndFinish();
}

RenderSequence::~RenderSequence() {
	delete fbo;
}

void RenderSequence::render(){
	fbo->bind();

	RenderEngine::Instance().clear();

	//Using the fixed pipeline to render to the depthbuffer
	//glUseProgram(0);

	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	fbo->updateRenderView();

	// Clear previous frame values
	//glClear( GL_DEPTH_BUFFER_BIT);

	//Disable color rendering, we only want to write to the Z-Buffer
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	//glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0,0.0);
    //glPolygonOffset(1.1, 4.0);
	pass2Mat->activate();
	SceneGraph::Instance().drawNodesLight(pass2Mat);
	fbo->unBind();

	glDisable(GL_POLYGON_OFFSET_FILL);
	//glCullFace(GL_BACK);
	RenderEngine::Instance().clear();

	//fbo->draw();


	glBindTexture(GL_TEXTURE_2D, 0);

	//fbo->updateRenderView();
	glViewport(0,0,1920, 1200);
	//Enabling color write (previously disabled for light POV z-buffer rendering)
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	pass1Mat->activate();

	if(RenderEngine::Instance().lightView){
		SceneGraph::Instance().drawNodesLight(pass1Mat);
	}else{
		SceneGraph::Instance().drawNodes(pass1Mat);
	}


	glError("RenderSequence::draw", 66);
}
