/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderPasses.h
 *
 *  Created on: Oct 5, 2010
 */

#pragma once

#include "Scene/Light.h"
#include "Renderer/FrameBuffer.h"

class DrawThing {
public:
  virtual void draw() = 0;
  static void drawOnPlane(Material * material, Mesh *plane);
};

class DrawPass : public DrawThing{
public:
  QSize res;
  Material * material;
  explicit DrawPass(QSize res);
};

class OnePass {
public:
  static void draw();
};

class SourcePass : public DrawPass {
public:
  vector<Texture*> targets;
  FrameBuffer * fbo;
  SourcePass(QSize res, vector<Texture*> &targets, Material * material);
  void initFBO();
  void draw();
  Texture* getTarget(string target);
};

class ShadowCastPass : public SourcePass {
public:
  Light* view;
  GLfloat offsetFactor;
  GLfloat offsetUnits;
  GLenum offsetMode;
  ShadowCastPass(QSize res, vector<Texture*> &targets, Light* view);
  void draw();
  void setOffsetFactor(GLfloat factor);
  void setOffsetUnits(GLfloat units);
  void setOffsetMode(GLenum offsetMode);
};

class InOutPass : public SourcePass {
public:
  Mesh * fullPlane;
  vector<Texture*> sources;
  Texture* getSource(string target);
  InOutPass(QSize res, vector<Texture*> &sources, vector<Texture*> &targets, Material * material);
  void draw();
};

class ShadowReceivePass : public InOutPass {
public:
  ShadowReceivePass(QSize res, vector<Texture*> &sources, vector<Texture*> &targets, Material * material) : InOutPass(res, sources, targets, material) {

  }
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

class SinkPass : public DrawThing {
public:
  vector<DebugPlane*> debugPlanes;
  SinkPass();
  void debugTarget(QRectF rect, Texture * target);
  void draw();
};
