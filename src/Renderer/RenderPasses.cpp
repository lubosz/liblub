/*
 * RenderPasses.cpp
 *
 *  Created on: Oct 5, 2010
 *      Author: bmonkey
 */

#include "MediaLayer.h"
#include "Camera.h"
#include "Materials.h"

LightViewDepthPass::LightViewDepthPass(FrameBuffer * fbo) {
    this->fbo = fbo;
    targetTexture = TextureFactory::Instance().depthTexture(fbo->width,
            fbo->height, "shadowMap");
    fbo->attachTexture(GL_DEPTH_ATTACHMENT, targetTexture);
    fbo->disableColorBuffer();

    material = new Minimal();
}

void LightViewDepthPass::prepare() {
    fbo->bind();

    RenderEngine::Instance().clear();

    // In the case we render the shadowmap to a higher resolution,
    // the viewport must be modified accordingly.
    fbo->updateRenderView();
    glPolygonOffset(2.0, 0.0);
    material->activate();
}

void LightViewDepthPass::draw() {
    SceneGraph::Instance().drawCasters(material);
}

void LightViewDepthPass::cleanUp() {
    fbo->unBind();
    glPolygonOffset(0.0, 0.0);
}

FilterPass::FilterPass(FrameBuffer * fbo) {
    this->fbo = fbo;
    targetTexture = TextureFactory::Instance().depthTexture(fbo->width,
            fbo->height, "shadowMap");
    fbo->attachTexture(GL_DEPTH_ATTACHMENT, targetTexture);
    fbo->disableColorBuffer();

    material = new Minimal();
}

void FilterPass::prepare() {
    fbo->bind();

    RenderEngine::Instance().clear();

    // In the case we render the shadowmap to a higher resolution,
    // the viewport must be modified accordingly.
    fbo->updateRenderView();
    SceneGraph::Instance().drawNodes(&Camera::Instance());
    fbo->unBind();

    glPolygonOffset(2.0, 0.0);
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
    glViewport(0, 0, MediaLayer::Instance().width,
            MediaLayer::Instance().height);
    if (RenderEngine::Instance().lightView) {
        SceneGraph::Instance().drawNodes(SceneGraph::Instance().light);
    } else {
        SceneGraph::Instance().drawNodes(&Camera::Instance());
    }
}

void LightTogglePass::cleanUp() {
}
