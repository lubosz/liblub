/*
 * RenderPasses.h
 *
 *  Created on: Oct 5, 2010
 *      Author: bmonkey
 */

#pragma once

#include "RenderPass.h"

class LightViewDepthPass : public RenderPass {
public:
	LightViewDepthPass(FrameBuffer * fbo);
	void prepare();
	void draw();
	void cleanUp();
};

class LightTogglePass : public RenderPass {
public:
	LightTogglePass();
	void prepare();
	void draw();
	void cleanUp();
};
