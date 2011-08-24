/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderSequence.cpp
 *
 *  Created on: Oct 4, 2010
 */

#include "Renderer/RenderSequence.h"
#include "Window/Window.h"
#include "System/Logger.h"
#include "Scene/SceneData.h"

RenderSequence::RenderSequence() {
    QSize res = SceneData::Instance().getResolution();
    fbo = new FrameBuffer(res);
    renderPasses.push_back(new ShadowPass(fbo));
    renderPasses.push_back(new LightTogglePass());
    fbo->check();
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
