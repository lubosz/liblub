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
    int m_nSamples;
//    GLenum m_nPolygonMode;
    float m_Kr, m_Kr4PI;
    float m_Km, m_Km4PI;
    float m_ESun;
    float m_g;
    float m_fExposure;

    float m_fInnerRadius;
    float m_fOuterRadius;
    float m_fScale;
    float m_fWavelength[3];
    float m_fWavelength4[3];
    float m_fRayleighScaleDepth;
    float m_fMieScaleDepth;

    Camera* camera;
    Light * light;
    Node * spaceNode;
  Node * groundNode;
  Node * skyNode;
    QVector3D lightDirection;
    FrameBuffer *fbo;
    Texture * targetTexture;

  explicit AtmosphereApp(string sceneName) {
    useHDR = true;
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~AtmosphereApp() {}

  void setAtmoUniforms(ShaderProgram * program){
    program->use();
    program->setUniform("v3LightPos", lightDirection);
    program->setUniform("v3InvWavelength", QVector3D(1/m_fWavelength4[0], 1/m_fWavelength4[1], 1/m_fWavelength4[2]));
    program->setUniform("fInnerRadius", m_fInnerRadius);
    program->setUniform("fInnerRadius2", m_fInnerRadius*m_fInnerRadius);
    program->setUniform("fOuterRadius", m_fOuterRadius);
    program->setUniform("fOuterRadius2", m_fOuterRadius*m_fOuterRadius);
    program->setUniform("fKrESun", m_Kr*m_ESun);
    program->setUniform("fKmESun", m_Km*m_ESun);
    program->setUniform("fKr4PI", m_Kr4PI);
    program->setUniform("fKm4PI", m_Km4PI);
    program->setUniform("fScale", 1.0f / (m_fOuterRadius - m_fInnerRadius));
    program->setUniform("fScaleDepth", m_fRayleighScaleDepth);
    program->setUniform("fScaleOverScaleDepth", (1.0f / (m_fOuterRadius - m_fInnerRadius)) / m_fRayleighScaleDepth);
    program->setUniform("g", m_g);
    program->setUniform("g2", m_g*m_g);
  }
  void setCameraUniforms(ShaderProgram * program){
    program->use();
    program->setUniform("v3CameraPos", camera->position);
    program->setUniform("fCameraHeight", (float)camera->position.length());
    program->setUniform("fCameraHeight2", float(camera->position.length()*camera->position.length()));
  }

  void scene() {
    m_fWavelength[0] = 0.650f;    // 650 nm for red
    m_fWavelength[1] = 0.570f;    // 570 nm for green
    m_fWavelength[2] = 0.475f;    // 475 nm for blue
    m_fWavelength4[0] = powf(m_fWavelength[0], 4.0f);
    m_fWavelength4[1] = powf(m_fWavelength[1], 4.0f);
    m_fWavelength4[2] = powf(m_fWavelength[2], 4.0f);

    QVector3D lightPosition = QVector3D(0, 0, 1000);
    lightDirection = lightPosition / lightPosition.length();

    m_fInnerRadius = 10.0f;
    m_fOuterRadius = 10.25f;
    m_fScale = 1 / (m_fOuterRadius - m_fInnerRadius);

    m_nSamples = 3;   // Number of sample rays to use in integral equation
    m_Kr = 0.0025f;   // Rayleigh scattering constant
    m_Kr4PI = m_Kr*4.0f*M_PI;
    m_Km = 0.0010f;   // Mie scattering constant
    m_Km4PI = m_Km*4.0f*M_PI;
    m_ESun = 20.0f;   // Sun brightness constant
    m_g = -0.990f;    // The Mie phase asymmetry factor
    m_fExposure = 2.0f;

    m_fRayleighScaleDepth = 0.25f;
    m_fMieScaleDepth = 0.1f;


    sceneLoader->load();
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
    light = new Light(QVector3D(-2.5, 21.5, -5.2), QVector3D(1, -5, 0));
    SceneData::Instance().addLight("foolight", light);


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

    Mesh * spacePlane = new Mesh();
    spacePlane->init();
    spacePlane->addBuffer(vertices, 3, "in_Vertex");
    spacePlane->addBuffer(uvCoords, 2, "in_Uv");
    spacePlane->addElementBuffer(indicies);
    spacePlane->setDrawType(GL_TRIANGLES);
//    Mesh * innerSphere = Geometry::gluSphere(m_fInnerRadius, 100, 50);
    Mesh * outerSphere = Geometry::gluSphere(m_fOuterRadius, 100, 50);
    Mesh * innerSphere = MeshFactory::load("sphere.obj");

    spaceNode = new Node("space", { 0, 0, 0 }, 1, spacePlane, spaceFromAtmosphere);
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

      HDR = new Template("Atmo/HDR",attributes);
      HDR->addTexture(targetTexture);
      HDR->shaderProgram->setUniform("fExposure", m_fExposure);
    }
  }
  void renderFrame(){
    if(useHDR)
      fbo->bind();
    RenderEngine::Instance().clear();

//    bool drawSpace = false;
//    if(camera->position.length() < m_fOuterRadius) {
//      spaceNode->setMaterial(spaceFromAtmosphere);
//      drawSpace = true;
//    }else if(camera->position.z() > 0.0f) {
//      spaceNode->setMaterial(spaceFromSpace);
//      drawSpace = true;
//    }
//
//    if(drawSpace) {
//      setCameraUniforms(spaceNode->getMaterial()->getShaderProgram());
//      spaceNode->setView(camera);
//      spaceNode->draw();
//    }

    if(camera->position.length() >= m_fOuterRadius)
      groundNode->setMaterial(groundFromSpace);
    else
      groundNode->setMaterial(groundFromAtmosphere);

    setCameraUniforms(groundNode->getMaterial()->getShaderProgram());
    groundNode->setView(camera);
    groundNode->draw();

//    if(camera->position.length() >= m_fOuterRadius)
//      skyNode->setMaterial(skyFromSpace);
//    else
//      skyNode->setMaterial(skyFromAtmosphere);
//
//    setCameraUniforms(skyNode->getMaterial()->getShaderProgram());
//    glFrontFace(GL_CW);
//    glEnable(GL_BLEND);
//
//    glBlendFunc(GL_ONE, GL_ONE);
//    skyNode->setView(camera);
//    skyNode->draw();
//
//    glDisable(GL_BLEND);
//    glFrontFace(GL_CCW);

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
  AtmosphereApp("atmo").run();
}

