/*
 * Copyright © 2010 Lubosz Sarnecki
 * RenderPass.cpp
 *
 *  Created on: Sep 10, 2010
 */

#include "RenderPass.h"

RenderPass::RenderPass() {
    // TODO(bmonkey): Auto-generated constructor stub
}

RenderPass::~RenderPass() {
    // TODO(bmonkey): Auto-generated destructor stub
}

void RenderPass::render() {
    prepare();
    draw();
    cleanUp();
}
