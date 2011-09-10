/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderPasses.cpp
 *
 *  Created on: Oct 5, 2010
 */

#include "RenderPasses.h"
#include "Window/Window.h"
#include "Scene/Camera.h"
#include "Material/Textures.h"
#include "Scene/SceneData.h"
#include "Scene/SceneGraph.h"
#include "Material/Materials.h"
#include "Mesh/Geometry.h"
#include "System/TemplateEngine.h"

  void DrawThing::drawOnPlane(Material * material, Mesh *plane) {
    material->getShaderProgram()->use();
    material->activateAndBindTextures();
    material->getShaderProgram()->setUniform("MVPMatrix", QMatrix4x4());
    plane->draw();
  }

  DrawPass::DrawPass(QSize res) {
    this->res = res;
  }

  SourcePass::SourcePass(QSize res, vector<Texture*> &targets, Material * material) : DrawPass(res) {
    this->targets = targets;
    this->material = material;
    initFBO();
    if(targets.size() > 1)
      material->initRenderTargets(targets);
  }

  void OnePass::draw(){
      RenderEngine::Instance().clear();
      SceneGraph::Instance().drawNodes();
  }

  void SourcePass::draw() {
    fbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    SceneGraph::Instance().drawNodes(material);
    fbo->unBind();
  }

  Texture* SourcePass::getTarget(string target) {
    foreach (Texture * texture, targets) {
        if (texture->name == "")
          LogFatal << "No Texture Name!";
        if (texture->name == target) {
          return texture;
        }
    }
    LogFatal << "Texture Not Found" << target;
    return new Texture();
  }

  void SourcePass::initFBO() {
    fbo = new FrameBuffer(res);
    fbo->attachTextures(targets);
    fbo->check();
  }

  ShadowCastPass::ShadowCastPass(QSize res, vector<Texture*> &targets, Light* view)
  : SourcePass(res, targets, new Minimal()) {
    offsetFactor = 2;
    offsetUnits = 0;
    offsetMode = GL_POLYGON_OFFSET_FILL;
    this->view = view;
    fbo->disableColorBuffer();
  }

  void ShadowCastPass::draw() {
    fbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(offsetFactor, offsetUnits);
    SceneGraph::Instance().drawCasters(material, view);
    glPolygonOffset(0.0, 0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
    fbo->unBind();
  }

  void ShadowCastPass::setOffsetFactor(GLfloat factor) {
    offsetFactor = factor;
  }
  void ShadowCastPass::setOffsetUnits(GLfloat units) {
    offsetUnits = units;
  }
  void ShadowCastPass::setOffsetMode(GLenum offsetMode) {
    this->offsetMode = offsetMode;
  }

  void ShadowReceivePass::draw() {
    fbo->bind();
    material->activateTextures();
    material->bindTextures();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    SceneGraph::Instance().drawReceivers(material);
    fbo->unBind();
  }

  InOutPass::InOutPass(QSize res, vector<Texture*> &sources, vector<Texture*> &targets, Material * material) : SourcePass(res, targets, material) {
    this->sources = sources;
    fullPlane = Geometry::plane(QList<string> () << "uv", QRectF(-1, -1, 2, 2));
    material->addTextures(sources);
    material->samplerUniforms();
    material->activateAndBindTextures();
  }

  Texture* InOutPass::getSource(string target) {
    foreach (Texture * texture, sources) {
        if (texture->name == "")
          LogFatal << "No Texture Name!";
        if (texture->name == target) {
          return texture;
        }
    }
    LogFatal << "Texture Not Found" << target;
    return new Texture();
  }


  void InOutPass::draw() {
    fbo->bind();
    RenderEngine::Instance().clear();
    RenderEngine::Instance().updateViewport(res);
    drawOnPlane(material, fullPlane);
    fbo->unBind();
  }

  DebugPlane::DebugPlane(QRectF rect, Texture * target){
    plane = Geometry::plane(QList<string> () << "uv", rect);
    material = initDebugMaterial(target);
  }

  Material * DebugPlane::initDebugMaterial(Texture * target) {
    TemplateEngine::Instance().c.insert("samplerName", QString::fromStdString(target->name));
    Material * debugMaterial = new Template("Post/Debug", QList<string> () << "uv");
    debugMaterial->addTexture(target);
    debugMaterial->samplerUniforms();
    debugMaterial->activateAndBindTextures();
    return debugMaterial;
  }

  void DebugPlane::draw(){
    drawOnPlane(material, plane);
  }

  SinkPass::SinkPass() {}

  void SinkPass::debugTarget(QRectF rect, Texture * target) {
    debugPlanes.push_back(new DebugPlane(rect, target));
  }

  void SinkPass::draw() {
    RenderEngine::Instance().clear();
    foreach(DebugPlane *plane, debugPlanes) {
      plane->draw();
    }
  }
