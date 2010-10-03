/*
 * RenderSequence.cpp
 *
 *  Created on: Oct 4, 2010
 *      Author: bmonkey
 */

#include "RenderSequence.h"

RenderSequence::RenderSequence() {
    fbo = new FrameBuffer(1920,1200);

	pass1Mat = new ShadowMap(1920, 1200);
	//pass1Mat = new FBOMaterial(width, height);
	pass2Mat = new ShadowMapDepth();

	fbo->attachTexture(GL_COLOR_ATTACHMENT0, pass1Mat->textures[0]);
	fbo->checkAndFinish();

	/*
	Texture * pass1 = TextureFactory::Instance().texture(width, height, "RenderTexture");
	Texture * pass2 = TextureFactory::Instance().texture(width, height, "RenderTexture");

	pass1Mat = new FBOMaterial();
	pass1Mat->addTexture(pass1);
	pass1Mat->done();

	pass2Mat = new FBOMaterial();
	pass2Mat->addTexture(pass2);
	pass2Mat->done();
*/

}

RenderSequence::~RenderSequence() {
	delete fbo;
}

void RenderSequence::render(){
	fbo->bind();

	RenderEngine::Instance().clear();

	glError("FrameBuffer::draw", 105);

	//Using the fixed pipeline to render to the depthbuffer
	//glUseProgram(0);

	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	glViewport(0,0,fbo->width, fbo->height);

	// Clear previous frame values
	glClear( GL_DEPTH_BUFFER_BIT);

	//Disable color rendering, we only want to write to the Z-Buffer
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	glCullFace(GL_FRONT);
	pass2Mat->activate();
	SceneGraph::Instance().drawNodesLight(pass2Mat);
	fbo->unBind();

	glCullFace(GL_BACK);
	RenderEngine::Instance().clear();

	glBindTexture(GL_TEXTURE_2D, 0);

	glViewport(0,0,fbo->width, fbo->height);

	pass1Mat->activate();

	if(RenderEngine::Instance().lightView){
		SceneGraph::Instance().drawNodesLight(pass1Mat);
	}else{
		SceneGraph::Instance().drawNodes(pass1Mat);
	}
	//renderPlane->draw();
	glError("FrameBuffer::draw", 171);
}
