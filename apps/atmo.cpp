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
#include "Mesh/Geometry.h"
#include "Mesh/MeshFactory.h"
#include "Material/ProcTextures.h"
#include "Material/ShaderProgram.h"

class AtmosphereApp: public Application {
 public:
  Material *groundFromAtmosphere, *groundFromSpace,
                *skyFromAtmosphere, *skyFromSpace,
                *spaceFromAtmosphere,*spaceFromSpace,
                *HDR;

  bool useHDR;
  float innerRadius;
  float outerRadius;
  Camera* camera;
  Light * light;
  Node * spaceNode,* groundNode,* skyNode;
  QVector3D lightDirection;
  FrameBuffer *fbo;
  Texture * targetTexture;

  AtmosphereApp() {
    useHDR = false;
    innerRadius = 10.0f;
    outerRadius = 10.25f;
  }

  ~AtmosphereApp() {}

  void setAtmoUniforms(ShaderProgram * program) {

    float wavelength[3];
    wavelength[0] = 0.650f; // 650 nm for red
    wavelength[1] = 0.570f; // 570 nm for green
    wavelength[2] = 0.475f; // 475 nm for blue

    float wavelength4[3];
    wavelength4[0] = powf(wavelength[0], 4.0f);
    wavelength4[1] = powf(wavelength[1], 4.0f);
    wavelength4[2] = powf(wavelength[2], 4.0f);

    QVector3D lightPosition = QVector3D(0, 0, 1000);
    lightDirection = lightPosition / lightPosition.length();

    float rayleigh = 0.0025f; // Rayleigh scattering constant
    float rayleigh4Pi = rayleigh * 4.0f * M_PI;
    float mie = 0.0010f; // Mie scattering constant
    float mie4Pi = mie * 4.0f * M_PI;
    float sun = 20.0f; // Sun brightness constant
    float g = -0.990f; // The Mie phase asymmetry factor
    float rayleighScaleDepth = 0.25f;
//    float mieScaleDepth = 0.1f;

    program->use();
    program->setUniform("v3LightPos", lightDirection);
    program->setUniform("v3InvWavelength", QVector3D(1 / wavelength4[0], 1 / wavelength4[1], 1 / wavelength4[2]));
    program->setUniform("fInnerRadius", innerRadius);
    program->setUniform("fInnerRadius2", innerRadius * innerRadius);
    program->setUniform("fOuterRadius", outerRadius);
    program->setUniform("fOuterRadius2", outerRadius * outerRadius);
    program->setUniform("fKrESun", rayleigh * sun);
    program->setUniform("fKmESun", mie * sun);
    program->setUniform("fKr4PI", rayleigh4Pi);
    program->setUniform("fKm4PI", mie4Pi);
    program->setUniform("fScale", 1.0f / (outerRadius - innerRadius));
    program->setUniform("fScaleDepth", rayleighScaleDepth);
    program->setUniform("fScaleOverScaleDepth", (1.0f / (outerRadius - innerRadius)) / rayleighScaleDepth);
    program->setUniform("g", g);
    program->setUniform("g2", g * g);
  }
  void setCameraUniforms(ShaderProgram * program){
    program->use();
    program->setUniform("v3CameraPos", camera->position);
    program->setUniform("fCameraHeight", (float)camera->position.length());
    program->setUniform("fCameraHeight2", float(camera->position.length()*camera->position.length()));
  }

  Mesh * moonPlane(){
    vector<GLfloat> vertices = {
        -4.0f, 4.0f, -50.0f,
        -4.0f, -4.0f, -50.0f,
        4.0f, -4.0f, -50.0f,
        4.0f, 4.0f, -50.0f
    };
    vector<GLfloat> uvCoords = {
            0.0, 0.0,
            0.0, 1.0,
            1.0, 1.0,
            1.0, 0.0,
    };

    vector<GLuint> indicies = { 0, 1, 3, 3, 1, 2 };

    Mesh * moonPlane = new Mesh();
    moonPlane->init();
    moonPlane->addBuffer(vertices, 3, "in_Vertex");
    moonPlane->addBuffer(uvCoords, 2, "in_Uv");
    moonPlane->addElementBuffer(indicies);
    moonPlane->setDrawType(GL_TRIANGLES);
    return moonPlane;
  }

  void scene() {
    Texture * glow = TextureFactory::Instance().load(ProcTextures::makeGlow(
        QSize(512, 512), 40.0f, 0.1f), "glow");
    Texture * earthMap = TextureFactory::Instance().load("earthmap1k.jpg",
        "planet");

    QList<string> attributes;
    attributes.push_back("uv");

    groundFromAtmosphere = new Template("Atmo/Ground",attributes);
    groundFromAtmosphere->addTexture(earthMap);
    spaceFromAtmosphere = new Template("Atmo/Space",attributes);
    spaceFromAtmosphere->addTexture(glow);
    skyFromAtmosphere = new Template("Atmo/Sky",attributes);

    TemplateEngine::Instance().c.insert("fromSpace", true);

    groundFromSpace = new Template("Atmo/Ground",attributes);
    groundFromSpace->addTexture(earthMap);
    spaceFromSpace = new Template("Atmo/Space",attributes);
    spaceFromSpace->addTexture(glow);
    skyFromSpace = new Template("Atmo/Sky",attributes);

    camera = SceneData::Instance().getCurrentCamera();
    camera->setPosition(QVector3D(0, 0, 25));
    camera->update();
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);

    Mesh * innerSphere = Geometry::gluSphere(innerRadius, 100, 50);
    Mesh * outerSphere = Geometry::gluSphere(outerRadius, 100, 50);

    spaceNode = new Node("space", { 0, 0, 0 }, 1, moonPlane(), spaceFromAtmosphere);
    groundNode = new Node("ground", { 0, 0, 0 }, 1, innerSphere, groundFromAtmosphere);
//    groundNode->setRotation(QVector3D(-90, 0, 180));
    skyNode = new Node("sky", { 0, 0, 0 }, 1, outerSphere, skyFromAtmosphere);
//    skyNode->setRotation(QVector3D(-90, 0, 180));

    setAtmoUniforms(spaceFromAtmosphere->getShaderProgram());
    setAtmoUniforms(groundFromAtmosphere->getShaderProgram());
    setAtmoUniforms(skyFromAtmosphere->getShaderProgram());
    setAtmoUniforms(spaceFromSpace->getShaderProgram());
    setAtmoUniforms(groundFromSpace->getShaderProgram());
    setAtmoUniforms(skyFromSpace->getShaderProgram());

    if(useHDR){
      unsigned width = MediaLayer::Instance().width;
      unsigned height = MediaLayer::Instance().height;

      fbo = new FrameBuffer(width, height);
      targetTexture = TextureFactory::Instance().colorTexture(width, height, "targetTexture");
      fbo->attachTexture(GL_COLOR_ATTACHMENT0, targetTexture);

      HDR = new Template("Post/HDR",attributes);
      HDR->addTexture(targetTexture);
      HDR->shaderProgram->setUniform("exposure", 2.0f);
      fbo->checkAndFinish();
    }
  }
  void renderFrame(){
    if(useHDR) {
      fbo->bind();
      fbo->updateRenderView();
    }
    RenderEngine::Instance().clear();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    bool drawSpace = false;
    if(camera->position.length() < outerRadius) {
      spaceNode->setMaterial(spaceFromAtmosphere);
      drawSpace = true;
    }else if(camera->position.z() > 0.0f) {
      spaceNode->setMaterial(spaceFromSpace);
      drawSpace = true;
    }

    if(drawSpace) {
      setCameraUniforms(spaceNode->getMaterial()->getShaderProgram());
      spaceNode->setView(camera);
      spaceNode->draw();
    }

    if(camera->position.length() >= outerRadius)
      groundNode->setMaterial(groundFromSpace);
    else
      groundNode->setMaterial(groundFromAtmosphere);

    setCameraUniforms(groundNode->getMaterial()->getShaderProgram());
    groundNode->setView(camera);
    groundNode->draw();

    if(camera->position.length() >= outerRadius)
      skyNode->setMaterial(skyFromSpace);
    else
      skyNode->setMaterial(skyFromAtmosphere);

    setCameraUniforms(skyNode->getMaterial()->getShaderProgram());
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);

    glBlendFunc(GL_ONE, GL_ONE);
    skyNode->setView(camera);
    skyNode->draw();

    glDisable(GL_BLEND);
    glFrontFace(GL_CCW);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    GUI::Instance().draw();
    glError;

    if(useHDR){
      fbo->unBind();
      RenderEngine::Instance().clear();
      HDR->activateTextures();
      HDR->getShaderProgram()->use();
      fbo->draw(HDR);
      glError;
    }
  }
};


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp().run();
}

