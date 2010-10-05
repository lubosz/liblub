/*
 * RenderPass.cpp
 *
 *  Created on: Sep 10, 2010
 *      Author: bmonkey
 */

#include "RenderPass.h"

RenderPass::RenderPass() {
	// TODO Auto-generated constructor stub

}

RenderPass::~RenderPass() {
	// TODO Auto-generated destructor stub
}

void RenderPass::render(){
	prepare();
	draw();
	cleanUp();
}
