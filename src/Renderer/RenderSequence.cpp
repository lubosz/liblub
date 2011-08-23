/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderSequence.cpp
 *
 *  Created on: Oct 4, 2010
 */

#include "Renderer/RenderSequence.h"
#include "Window/MediaLayer.h"
#include "System/Logger.h"
#include "Scene/SceneData.h"

RenderSequence::RenderSequence() {
#ifdef USE_FBO
    unsigned width = SceneData::Instance().width;
    unsigned height = SceneData::Instance().height;

//    TODO: Hardcoded shadow map size
//    width = 4096;
//    height = 4096;
    QSize res = SceneData::Instance().getResolution();
    fbo = new FrameBuffer(res);

    // pass1Mat = new ShadowMapPhongPCFAmbient(width, height);
    // pass1Mat = new FBOMaterial(width, height);
    // minimal = new Minimal();

    // fbo->attachTexture(GL_COLOR_ATTACHMENT0, fbo->getDebugTexture());
    // fbo->attachTexture(GL_COLOR_ATTACHMENT0, pass1Mat->textures[0]);

    renderPasses.push_back(new ShadowPass(fbo));
//    renderPasses.push_back(new DepthPass(fbo));
#endif

    renderPasses.push_back(new LightTogglePass());
//    renderPasses.push_back(new FBODebugPass(fbo));
//    renderPasses.push_back(new FilterPass(fbo));

#ifdef USE_FBO
    fbo->check();
#endif
}

RenderSequence::~RenderSequence() {
    delete fbo;
}

void RenderSequence::render() {
    foreach(RenderPass * renderPass, renderPasses) {
            renderPass->render();
    }
    glError;
}
