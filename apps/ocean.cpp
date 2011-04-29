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
#include <math.h>
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

class AtmosphereApp: public Application {
 public:



  Camera* camera;
  Light * light;
  Node *oceanNode;
  Material * ocean;

  AtmosphereApp() {
  }

  ~AtmosphereApp() {}


  void setCameraUniforms(ShaderProgram * program){
    program->use();
    program->setUniform("cameraPosition", camera->position);
    program->setUniform("cameraHeight", (float)camera->position.length());
    program->setUniform("cameraHeight2", float(camera->position.length()*camera->position.length()));
  }



  void scene() {

    QList<string> attributes;
    attributes.push_back("normal");
    attributes.push_back("uv");

    camera = SceneData::Instance().getCurrentCamera();
    camera->setPosition(QVector3D(0, 0, 25));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);

    Mesh * innerSphere = Geometry::sphere(attributes, 11.0f, 100, 50);
//    Mesh * innerSphere = MeshFactory::plane();
//    Mesh * innerSphere = MeshFactory::load("earth.obj");

    ocean = new Template("Ocean",attributes);
    ocean->getShaderProgram()->use();
    ocean->getShaderProgram()->setUniform("deepColor", QVector4D(0, 0.3, 0.5, 1.0));
    ocean->getShaderProgram()->setUniform("shallowColor",QVector4D(0, 1, 1, 1.0));
    ocean->getShaderProgram()->setUniform("reflectionColor",QVector4D(0.95, 1, 1, 1.0));
    ocean->getShaderProgram()->setUniform("reflectionAmount",1.0f);
    ocean->getShaderProgram()->setUniform("reflectionBlur",0.0f);
    ocean->getShaderProgram()->setUniform("waterAmount",0.3f);
    ocean->getShaderProgram()->setUniform("fresnelPower",5.0f);
    ocean->getShaderProgram()->setUniform("fresnelBias",0.328f);
    ocean->getShaderProgram()->setUniform("hdrMultiplier", 0.471f);
    ocean->getShaderProgram()->setUniform("textureScale",QVector2D(25, 26));
    ocean->getShaderProgram()->setUniform("bumpSpeed",QVector2D(0.015, 0.005));
    ocean->getShaderProgram()->setUniform("BumpScale",0.2f);
    ocean->getShaderProgram()->setUniform("time",0.7f);
    ocean->getShaderProgram()->setUniform("waveFreq",0.028f);
    ocean->getShaderProgram()->setUniform("waveAmp",1.8f);

    Texture * oceanNormal = new TextureFile("ocean/waves.png","NormalMap");
    ocean->addTexture(oceanNormal);
    Texture * oceanSky = new CubeTextureFile("cubemaps/sky","EnvironmentMap");
    ocean->addTexture(oceanSky);

    oceanNode = new Node("ocean", { 0, 0, 0 }, 1, innerSphere, ocean);
    oceanNode->setRotation(QVector3D(-90,0,180));
  }
  void renderFrame(){

    RenderEngine::Instance().clear();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float time = float(Timer::Instance().secoundsPassed) + float(Timer::Instance().nanosecoundsPassed)/1000000000.0;
    oceanNode->setView(camera);
    ocean->getShaderProgram()->setUniform("time",time);
    ocean->getShaderProgram()->setUniform("eyePosition",camera->position);
    oceanNode->draw();

    GUI::Instance().draw();
    glError;
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp().run();
}

