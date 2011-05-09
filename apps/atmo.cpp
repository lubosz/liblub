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
#include <string>
#include <QApplication>
#include <QPainter>
#include "System/TemplateEngine.h"
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Mesh/Geometry.h"
#include "Mesh/MeshLoader.h"
#include "Material/ProcTextures.h"
#include "Material/Textures.h"
#include "Material/ShaderProgram.h"
#include "Planets/Planet.h"

class AtmosphereApp: public Application {
 public:
  Material *HDR;
  bool useHDR;
  Camera* camera;
  Light * light;

  FrameBuffer *fbo;
  Planet * planet, *sun, *terrain
//  , *terrainTess
  ;
  vector <Planet*> planets;

  AtmosphereApp() {
    useHDR = true;
//    planet = new Planet(11,11.55, Planet::ocean, QVector3D(0.650f, 0.570f,0.475f));
//    sun = new Planet(11,11.55, Planet::sun, QVector3D(0.650f,1,0));
//    terrain = new Planet(11,11.55, Planet::terrainPlain, QVector3D(0.650f,1,0.475f));
    terrain = new Planet(11,11.55, Planet::terrainPlain, QVector3D(0.650f, 0.570f,0.475f));
//    terrainTess = new Planet(11,11.55, Planet::terrainTess, QVector3D(0,1,0));
//    planets.push_back();
  }

  ~AtmosphereApp() {}


  void initCamAndLight(){
    camera = SceneData::Instance().getCurrentCamera();
//    camera->setPosition(QVector3D(0, 10, 7));
    camera->setPosition(QVector3D(0, 11.1, -0.85));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);

    camera->yaw = 2.9;
    camera->pitch = 176.6;
    camera->update();
    camera->updateView();
    camera->updateRotation();
    camera->updatePerspective();
  }

  void initPostProcessing(){
    if(useHDR){
      unsigned width = MediaLayer::Instance().width;
      unsigned height = MediaLayer::Instance().height;

      fbo = new FrameBuffer(width, height);
      Texture * targetTexture = new ColorTexture(width, height, "targetTexture");
      fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);

      QList<string> attributes;
      attributes.push_back("uv");

      HDR = new Template("Post/HDR",attributes);
      HDR->addTexture(targetTexture);
      HDR->shaderProgram->setUniform("exposure", 2.0f);
      fbo->checkAndFinish();
    }
  }

  void startPass(){

    useHDR = !RenderEngine::Instance().wire;

    if(useHDR) {
      fbo->bind();
      fbo->updateRenderView();
    }
  }

  void endPass(){
    if(useHDR){
      fbo->unBind();
      RenderEngine::Instance().clear();
      HDR->activateTextures();
      HDR->getShaderProgram()->use();
      fbo->draw(HDR);
    }
  }

  void scene() {
//    planet-> init({-30,0,0},1);
    terrain-> init({0,0,0},1);
//    terrainTess-> init({0,30,0},1);
//    sun-> init({0,0,1000},5);
    initCamAndLight();
    initPostProcessing();
  }

  void renderFrame(){
    startPass();
    RenderEngine::Instance().clear();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
//    planet->draw();
//    sun->draw();
    terrain->draw();
//    terrainTess->draw();

    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
//    planet->atmoSphere->draw();
//    sun->atmoSphere->draw();
    terrain->atmoSphere->draw();
//    terrainTess->atmoSphere->draw();

    glDisable(GL_BLEND);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    endPass();
    GUI::Instance().draw();
    glError;

//    LogDebug << camera->position.x() << camera->position.y() << camera->position.z();
//    LogDebug << camera->yaw << camera->pitch << camera->roll;
  }
};
#ifdef LIBLUB_WINDOWS
#include <windows.h>
int WINAPI WinMain(HINSTANCE inst,HINSTANCE prev,LPSTR cmd,int show) {
  LogDebug << "WINMAIN STUFF" << inst << prev << cmd << show;
  QApplication app();
  AtmosphereApp().run();
  return 0;
}
#else
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp().run();
  return 0;
}
#endif

