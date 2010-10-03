/*
 * RenderSequence.h
 *
 *  Created on: Oct 4, 2010
 *      Author: bmonkey
 */

#pragma once

#include "common.h"
#include "FrameBuffer.h"
#include "Materials.h"

class RenderSequence {
private:
	FrameBuffer *fbo;
	Material * pass1Mat, * pass2Mat;
public:
	RenderSequence();
	virtual ~RenderSequence();
	void render();
};

