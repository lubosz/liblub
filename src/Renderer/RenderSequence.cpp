/*
 * RenderSequence.cpp
 *
 *  Created on: Oct 4, 2010
 *      Author: bmonkey
 */

#include "MediaLayer.h"
#include <boost/foreach.hpp>

RenderSequence::RenderSequence() {
	unsigned width = MediaLayer::Instance().width;
	unsigned height = MediaLayer::Instance().height;

    fbo = new FrameBuffer(width,height);



	//pass1Mat = new ShadowMapPhongPCFAmbient(width, height);
	//pass1Mat = new FBOMaterial(width, height);
    //minimal = new Minimal();

	//fbo->attachTexture(GL_COLOR_ATTACHMENT0, fbo->getDebugTexture());
	//fbo->attachTexture(GL_COLOR_ATTACHMENT0, pass1Mat->textures[0]);

	renderPasses.push_back(new LightViewDepthPass(fbo));
	renderPasses.push_back(new LightTogglePass());

	fbo->checkAndFinish();
}

RenderSequence::~RenderSequence() {
	delete fbo;
}

void RenderSequence::render() {

	BOOST_FOREACH(RenderPass * renderPass, renderPasses){
		renderPass->render();
	}
	glError("RenderSequence::draw", 66);
}
