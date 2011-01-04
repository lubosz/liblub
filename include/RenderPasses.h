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
	explicit LightViewDepthPass(FrameBuffer * fbo);
	void prepare();
	void draw();
	void cleanUp();
};

class FilterPass : public RenderPass {
 public:
    explicit FilterPass(FrameBuffer * fbo);
	void prepare();
	void draw(Material * material);
	void cleanUp();
};

class LightTogglePass : public RenderPass {
 public:
	LightTogglePass();
	void prepare();
	void draw();
	void cleanUp();
};
