/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderPasses.cpp
 *
 *  Created on: Oct 5, 2010
 */

#include "Window/Window.h"
#include "Scene/Camera.h"
#include "Material/Textures.h"
#include "Scene/SceneData.h"
#include "Scene/SceneGraph.h"
#include "Material/Materials.h"
#include "Mesh/Geometry.h"
#include "System/TemplateEngine.h"

ShadowPass::ShadowPass(FrameBuffer * fbo) {
    this->fbo = fbo;
    targetTexture = new ShadowTexture(fbo->res, "shadowMap");
    fbo->attachDepthTexture(targetTexture);
    fbo->disableColorBuffer();

    material = new Minimal();
}

void ShadowPass::prepare() {
    fbo->bind();

    glPolygonOffset(2.0, 0.0);
    RenderEngine::Instance().clear();

    // In the case we render the shadowmap to a higher resolution,
    // the viewport must be modified accordingly.
    fbo->updateRenderView();
    material->activate();
}

void ShadowPass::draw() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  //TODO: Multiple lights
  SceneGraph::Instance().drawCasters(material,SceneData::Instance().getShadowLight());
}

void ShadowPass::cleanUp() {
    fbo->unBind();
    glPolygonOffset(0.0, 0.0);
    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
        glViewport(0, 0, SceneData::Instance().width,
            SceneData::Instance().height);
}
WritePass::WritePass(FrameBuffer * fbo, Texture * texture, Material * material) {
 WritePass(fbo, texture, material, true);
}


WritePass::WritePass(FrameBuffer * fbo, Texture * texture, Material * material, bool useColorBuffer) {
    this->fbo = fbo;
    this->material = material;
    targetTexture = texture;
    if (!useColorBuffer) {
      fbo->attachDepthTexture(targetTexture);
      fbo->disableColorBuffer();
    } else {
      fbo->attachTexture(targetTexture);
    }
}

void WritePass::prepare() {
    fbo->bind();
    RenderEngine::Instance().clear();
    fbo->updateRenderView();
    material->activate();
}

void WritePass::draw() {
    SceneGraph::Instance().drawCasters(material);
}

void WritePass::cleanUp() {
    fbo->unBind();
    glViewport(0, 0, SceneData::Instance().width,
        SceneData::Instance().height);
}

FilterPass::FilterPass(FrameBuffer * fbo) {
    this->fbo = fbo;
    QSize res = SceneData::Instance().getResolution();
    targetTexture = new DepthTexture(res, "shadowMap");
    fbo->attachDepthTexture(targetTexture);
    fbo->disableColorBuffer();

    material = new Minimal();
}

void FilterPass::prepare() {
    fbo->bind();

    RenderEngine::Instance().clear();

    // In the case we render the shadowmap to a higher resolution,
    // the viewport must be modified accordingly.
    fbo->updateRenderView();
    SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
    fbo->unBind();

    glPolygonOffset(20.0, 0.0);
    material->activate();

}

void FilterPass::draw(Material * material) {
    fbo->draw(material);
}

void FilterPass::cleanUp() {
    fbo->unBind();
    glPolygonOffset(0.0, 0.0);
}

LightTogglePass::LightTogglePass() {
}

void LightTogglePass::prepare() {
    RenderEngine::Instance().clear();
}

void LightTogglePass::draw() {

    if (RenderEngine::Instance().lightView) {
        SceneGraph::Instance().drawNodes(SceneData::Instance().getShadowLight());
    } else {
        SceneGraph::Instance().drawNodes(SceneData::Instance().getCurrentCamera());
    }
}

FBODebugPass::FBODebugPass(FrameBuffer * fbo) {
  this->fbo = fbo;
}

void FBODebugPass::prepare() {
    RenderEngine::Instance().clear();
}

void FBODebugPass::draw() {
  glViewport(0, 0, SceneData::Instance().width,
      SceneData::Instance().height);
  Material* material = SceneData::Instance().getMaterial("debugFBO");
//  foreach (Texture * texture, material->textures){
//    printf("Texture! %s\n", texture->name.c_str());
//  }
  fbo->draw(material);
}


//new render passes

  void DrawThing::drawOnPlane(Material * material, Mesh *plane) {
    material->getShaderProgram()->use();
    material->getShaderProgram()->setUniform("MVPMatrix", QMatrix4x4());
    material->activate();
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
  }

  void SourcePass::initFBO() {
    fbo = new FrameBuffer(res);
    fbo->attachTextures(targets);
    fbo->check();
  }


  InOutPass::InOutPass(QSize res, vector<Texture*> &sources, vector<Texture*> &targets, Material * material) : SourcePass(res, targets, material) {
    this->sources = sources;
    fullPlane = Geometry::plane(QList<string> () << "uv", QRectF(-1, -1, 2, 2));
    material->addTextures(sources);
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
    return debugMaterial;
  }

  void DebugPlane::draw(){
    drawOnPlane(material, plane);
  }

  SinkPass::SinkPass(QSize res, vector<Texture*> &targets, Material * material) : DrawPass(res){
    fullPlane = Geometry::plane(QList<string> () << "uv", QRectF(-1, -1, 2, 2));
    this->material = material;
    material->addTextures(targets);
  }

  void SinkPass::debugTarget(QRectF rect, Texture * target) {
    debugPlanes.push_back(new DebugPlane(rect, target));
  }

  void SinkPass::draw() {
    RenderEngine::Instance().clear();
    foreach(DebugPlane *plane, debugPlanes) {
      plane->draw();
    }
    drawOnPlane(material, fullPlane);
  }


