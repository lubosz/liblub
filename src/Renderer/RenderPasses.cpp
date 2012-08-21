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
#include "Scene/Scene.h"
#include "Scene/SceneGraph.h"
#include "Mesh/Geometry.h"
#include "System/TemplateEngine.h"
#include "Renderer/OpenGL.h"
#include "Material/Shaders.h"

DrawThing::DrawThing() : enabled(true), typeName("Pass"){
}

  void DrawThing::drawOnPlane(ShaderProgram * shader, Mesh *plane) {
    shader->use();
    shader->activateAndBindTextures();
    shader->setUniform("MVPMatrix", QMatrix4x4());
    plane->draw();
  }

  DrawPass::DrawPass(QSize res) : DrawThing() {
    this->res = res;
  }

  SourcePass::SourcePass(QSize res, vector<Texture*> &targets, ShaderProgram * shader) : DrawPass(res) {
    typeName = "SourcePass";
    this->targets = targets;
    this->shader = shader;
    initFBO();
    if(targets.size() > 1)
      shader->initRenderTargets(targets);
  }

  void OnePass::draw(){
      OpenGL::Instance().clear();
      SceneGraph::Instance().drawNodes();
  }

  void SourcePass::draw() {
    fbo->bind();
    OpenGL::Instance().clear();
    OpenGL::Instance().updateViewport(res);
    SceneGraph::Instance().drawNodes(shader);
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
    return new Texture("Dummy Texture");
  }

  void SourcePass::initFBO() {
    fbo = new FrameBuffer(res);
    fbo->attachTextures(targets);
    fbo->check();
  }

  ShadowCastPass::ShadowCastPass(QSize res, vector<Texture*> &targets, Light* view)
  : SourcePass(res, targets, new MinimalProgram()) {
    typeName = "ShadowCastPass";
    offsetFactor = 2;
    offsetUnits = 0;
    offsetMode = GL_POLYGON_OFFSET_FILL;
    this->view = view;
    fbo->disableColorBuffer();
  }

  void ShadowCastPass::draw() {
    fbo->bind();
    OpenGL::Instance().clear();
    OpenGL::Instance().updateViewport(res);
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(offsetFactor, offsetUnits);
    SceneGraph::Instance().drawCasters(shader, view);
    glPolygonOffset(0.0, 0.0);
    glCullFace(GL_BACK);
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

    OpenGL::Instance().clear();
    OpenGL::Instance().updateViewport(res);
    SceneGraph::Instance().drawReceivers(shader);
    fbo->unBind();
  }

  InOutPass::InOutPass(QSize res, vector<Texture*> &sources, vector<Texture*> &targets, ShaderProgram * shader) : SourcePass(res, targets, shader) {
    typeName = "InOutPass";
    this->sources = sources;
    fullPlane = Geometry::plane(QList<string> () << "uv", QRectF(-1, -1, 2, 2));
    shader->addTextures(sources);
    shader->samplerUniforms();
    shader->activateAndBindTextures();
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
    return new Texture("Dummy Texture");
  }

  void InOutPass::draw() {
    fbo->bind();
    OpenGL::Instance().clear();
    OpenGL::Instance().updateViewport(res);
    shader->use();
    shader->setUniform("camPositionWorld",Scene::Instance().getCurrentCamera()->getPosition());
    drawOnPlane(shader, fullPlane);
    fbo->unBind();
  }

  DebugPlane::DebugPlane(QRectF rect, Texture * target){
    targetName = target->name;
    visible = true;
    plane = Geometry::plane(QList<string> () << "uv", rect);
    shader = initDebugMaterial(target);
  }

  void DebugPlane::updateSource(Texture * target) {
    targetName = target->name;
    delete shader;
    shader = initDebugMaterial(target);
  }

  ShaderProgram * DebugPlane::initDebugMaterial(Texture * target) {
      TemplateEngine::Instance().c.insert("samplerName", QString::fromStdString(target->name));

    TemplateEngine::Instance().c.insert("isDepthTexture", target->isDepth);

    ShaderProgram * debugMaterial = new TemplateProgram("Post/Debug", QList<string> () << "uv");
    debugMaterial->addTexture(target);
    debugMaterial->samplerUniforms();
    debugMaterial->activateAndBindTextures();
    return debugMaterial;
  }

  void DebugPlane::draw(){
    if (visible)
      drawOnPlane(shader, plane);
  }

  SinkPass::SinkPass() {
    typeName = "SinkPass";
  }

  void SinkPass::debugTarget(QRectF rect, Texture * target) {
    debugPlanes.push_back(new DebugPlane(rect, target));
  }

  void SinkPass::draw() {
    OpenGL::Instance().clear();
    foreach(DebugPlane *plane, debugPlanes) {
      plane->draw();
    }
  }
