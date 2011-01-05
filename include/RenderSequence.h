/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderSequence.h
 *
 *  Created on: Oct 4, 2010
 */

#pragma once

#include "RenderPasses.h"

class RenderSequence {
 private:
	FrameBuffer *fbo;
	Material * minimal;

 public:
	vector<RenderPass*> renderPasses;

	RenderSequence();
	virtual ~RenderSequence();
	void render();
};

