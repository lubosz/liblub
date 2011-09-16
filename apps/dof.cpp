/*
    Copyright © 2010 Lubosz Sarnecki

    This file is part of liblub.

    liblub is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    liblub is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with liblub.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "System/Application.h"
#include "Material/Textures.h"
#include "Renderer/RenderEngine.h"
#include "Renderer/FrameBuffer.h"
#include "Scene/SceneLoader.h"
#include "Material/Shaders.h"
#include "Scene/Scene.h"

class DepthOfFieldExample: public Application
{
 public:
  SceneLoader *sceneLoader;

	ShaderProgram* pDebugDepthFBOMaterial;
	ShaderProgram* pDebugColorFBOMaterial;

	ShaderProgram* pDepthMaterial;
	ShaderProgram* pDOFShader;
	//Material* colorMaterial;

	FrameBuffer* pFBOColor;
	FrameBuffer* pFBODepth;

	QSize res;

	DepthOfFieldExample(int argc, char *argv[]) : Application(argc,argv) {
	  fontOverlay = true;
	  sceneLoader = new SceneLoader("terrain.xml");
  }

  ~DepthOfFieldExample() {}

void scene() {
    sceneLoader->load();
    res = Scene::Instance().getResolution();
    Scene::Instance().name = "Depth Buffer";

    Texture* pDepthTexture = new DepthTexture(res, "depth_map"); // debug? => "result"
    Texture* pColorTexture = new ColorTexture(res, "color_map");
    //Texture* pColorTextureS = new ColorTexture(res, "color_map_small");

    // fbo color
    pFBOColor = new FrameBuffer(res);
    pFBOColor->attachTexture(pColorTexture);
    //pFBOColor->attachTexture(GL_COLOR_ATTACHMENT1, pColorTextureS);
    pFBOColor->check();

    // fbo depth
    pFBODepth = new FrameBuffer(res);
    pFBODepth->attachTexture(pDepthTexture);
    pFBODepth->disableColorBuffer();
    pFBODepth->check();

    // shader attributes
    QList<string> attributes;
    attributes.push_back("uv");

    // depth material
    pDepthMaterial = new SimpleProgram("Common/depth",QList<string>());
    // = new Simple("Common/minimal",QList<string>());

    // debug depth
    pDebugDepthFBOMaterial = new SimpleProgram("Texture/debugfbo",attributes);
	pDebugDepthFBOMaterial->addTexture(pDepthTexture);

	// debug color
	pDebugColorFBOMaterial = new SimpleProgram("Texture/debugfbo",attributes);
	pDebugColorFBOMaterial->addTexture(pColorTexture);

	// DOF-Shader
	pDOFShader = new SimpleProgram("DepthOfField/dof", attributes);
	pDOFShader->addTexture(pColorTexture);
	pDOFShader->addTexture(pDepthTexture);
	pDOFShader->samplerUniforms();
  }
  void renderFrame()
  {
	// create depth texture
    RenderEngine::Instance().clear();
    pFBODepth->bind();
    RenderEngine::Instance().clear();
    pDepthMaterial->activateAndBindTextures();
    RenderEngine::Instance().updateViewport(res);
    SceneGraph::Instance().drawNodes(pDepthMaterial);
    pFBODepth->unBind();

    // create color texture
    RenderEngine::Instance().clear();
	pFBOColor->bind();
	RenderEngine::Instance().clear();
	SceneGraph::Instance().drawNodes();
	pFBOColor->unBind();

	// post processing
	//pDOFShader->setUniform("PMatrix", SceneData::Instance()..getCurrentCamera()->getProjection());
	pDOFShader->use();
	pDOFShader->setUniform("pixel_width", 1.f / res.width());
	pDOFShader->setUniform("pixel_height", 1.f / res.height());

	//pDOFShader->setUniform("VMatrix", QMatrix4x4());
	pDOFShader->setUniform("VMatrix", Scene::Instance().getCurrentCamera()->getView());
	pDOFShader->setUniform("PMatrix", Scene::Instance().getCurrentCamera()->getProjection());
	pDOFShader->setUniform("VPIMatrix", (Scene::Instance().getCurrentCamera()->getProjection() * Scene::Instance().getCurrentCamera()->getView()).inverted());

	/*TODO MVP-Inverse should be a uniform (now calculated)
	QMatrix4x4 tempMatrix = viewPoint->getView() * modelMatrix;
	    shaderProgram->setUniform("MMatrix", modelMatrix);
	    shaderProgram->setUniform("MVMatrix", tempMatrix);
	    shaderProgram->setUniform("NormalMatrix", tempMatrix.normalMatrix());
	    tempMatrix = viewPoint->getProjection() * tempMatrix;
	    shaderProgram->setUniform("MVPMatrix", tempMatrix);
	pDOFShader->setUniform("MVPIMatrix", SceneData::Instance().getCurrentCamera()->projectionMatrix);
	*/


	// pass 1
/*
	for(int i = 0; i < 8; i++)
	{
		pFBOColor->bind();
		pFBOColor->draw(pDOFShader);
		pFBOColor->unBind();
	}
*/

	// final pass
	pFBOColor->draw(pDOFShader);

  }
};

int main(int argc, char *argv[]) {
	DepthOfFieldExample(argc,argv).run();
}

