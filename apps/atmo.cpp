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
  vector <Planet*> planets;

  AtmosphereApp() {
    //   wavelength[0] = 0.650f; // 650 nm for red
    //    wavelength[1] = 0.570f; // 570 nm for green
    //    wavelength[2] = 0.475f; // 475 nm for blue
    useHDR = true;
    planets.push_back(new Planet(11,11.55, Planet::ocean, {0.650f, 0.570f,0.475f},{50,0,0},1));
    planets.push_back(new Planet(11,11.55, Planet::sun, {0.650f,1,0},{0,0,50},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainTess, {0.150f, 0.870f,0.175f},{-10,0,0},1));
//    planets.push_back(new Planet(11,11.55, Planet::terrainPlain, {0.650f, 0.570f,0.475f},{0,0,0},1));
    planets.push_back(new Planet(11,11.55, Planet::terrainPlain, {0.150f, 0.570f,0.475f},{0,0,0},1));
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
    foreach(Planet * planet, planets)
        planet->init();
    initCamAndLight();
    initPostProcessing();
  }

  void drawPlanets() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    foreach(Planet * planet, planets)
        planet->draw();

    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    foreach(Planet * planet, planets)
        planet->atmoSphere->draw();

    glDisable(GL_BLEND);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
  }

  void renderFrame(){
    startPass();
    RenderEngine::Instance().clear();
    drawPlanets();
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

