/*
 * RenderPass.cpp
 *
 *  Created on: Sep 10, 2010
 *      Author: bmonkey
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
