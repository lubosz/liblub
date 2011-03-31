/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderPasses.h
 *
 *  Created on: Oct 5, 2010
 */

#pragma once

#include "RenderPass.h"

class ShadowPass : public RenderPass {
 public:
	explicit ShadowPass(FrameBuffer * fbo);
	void prepare();
	void draw();
	void cleanUp();
};

class DepthPass : public RenderPass {
 public:
  explicit DepthPass(FrameBuffer * fbo);
  void prepare();
  void draw();
  void cleanUp();
};

class FilterPass : public RenderPass {
 public:
  explicit FilterPass(FrameBuffer * fbo);
	void prepare();
	void draw() {};
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

class FBODebugPass : public RenderPass {
 public:
  explicit FBODebugPass(FrameBuffer * fbo);
  void prepare();
  void draw();
  void cleanUp();
};
