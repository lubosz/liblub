/*
 * RenderPass.h
 *
 *  Created on: Sep 10, 2010
 *      Author: bmonkey
 */

#pragma once

#include "Material.h"
#include "FrameBuffer.h"

class RenderPass {
 public:
	Material * material;
	FrameBuffer * fbo;
	Texture * targetTexture;
	RenderPass();
	virtual ~RenderPass();
	virtual void draw() = 0;
	virtual void cleanUp() = 0;
	virtual void prepare() = 0;
	void render();
};
