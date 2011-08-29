/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderPasses.h
 *
 *  Created on: Oct 5, 2010
 */

#pragma once

#include "RenderPass.h"


//TODO: deprecated
class ShadowPass : public RenderPass {
 public:
	explicit ShadowPass(FrameBuffer * fbo);
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
	void cleanUp(){};
};

class FBODebugPass : public RenderPass {
 public:
  explicit FBODebugPass(FrameBuffer * fbo);
  void prepare();
  void draw();
  void cleanUp(){};
};

class WritePass : public RenderPass {
 public:
  WritePass(FrameBuffer * fbo, Texture * texture, Material * material);
  WritePass(FrameBuffer * fbo, Texture * texture, Material * material, bool useColorBuffer);
  void prepare();
  void draw();
  void cleanUp();
};

class DrawThing {
public:
  virtual void draw() = 0;
  static void drawOnPlane(Material * material, Mesh *plane);
};

class DrawPass : public DrawThing{
public:
  QSize res;
  explicit DrawPass(QSize res);
};

class OutPass : public DrawPass {
public:
  Material *material;
  FrameBuffer * fbo;
  explicit OutPass(QSize res);
  void draw();
};

class InOutPass : public OutPass {
public:
  Mesh * fullPlane;
  explicit InOutPass(QSize res);
  void draw();
};

class DebugPlane : public DrawThing{
public:
  Material * material;
  Mesh * plane;
  DebugPlane(QRectF rect, Texture * target);
  Material * initDebugMaterial(Texture * target);
  void draw();
};

class SinkPass : public DrawPass {
public:
  Material * material;
  Mesh * fullPlane;
  vector<DebugPlane*> debugPlanes;
  SinkPass(QSize res);
  void debugTarget(QRectF rect, Texture * target);
  void draw();
};
