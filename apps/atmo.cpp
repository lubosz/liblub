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
#include "System/Application.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneData.h"
#include "System/Logger.h"
#include <QPainter>
#include "Mesh/Geometry.h"
#include "Material/ProcTextures.h"
#include "Material/ShaderProgram.h"

class AtmosphereApp: public Application {
 public:
  Material *groundFromAtmosphere, *groundFromSpace,
                *skyFromAtmosphere, *skyFromSpace,
                *spaceFromAtmosphere,*spaceFromSpace,
                *HDR;

  bool m_bUseHDR;
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
    QVector3D lightDirection;

  explicit AtmosphereApp(string sceneName) {
    QString sceneFile = QString::fromStdString(sceneName + ".xml");
    sceneLoader = new SceneLoader(sceneFile);
  }

  ~AtmosphereApp() {}

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
        "myTexture");

//    Material * foo = new Simple("Atmo/SpaceFromSpace");
//    foo->addTexture(glow);
//    if(foo==NULL) printf("1");
    groundFromAtmosphere = new Simple("Atmo/GroundFromAtmosphere");
//    groundFromAtmosphere->addTexture(earthMap);
    groundFromSpace = new Simple("Atmo/GroundFromSpace");
//    groundFromSpace->addTexture(earthMap);
//    skyFromAtmosphere = new Simple("Atmo/SkyFromAtmosphere");
//    skyFromSpace = new Simple("Atmo/SkyFromSpace");
    spaceFromAtmosphere = new Simple("Atmo/SpaceFromAtmosphere");
    spaceFromAtmosphere->addTexture(glow);
    spaceFromSpace = new Simple("Atmo/SpaceFromSpace");
    spaceFromSpace->addTexture(glow);

//    HDR = new ShaderProgram();
//    HDR->attachShader("Atmo/HDR.vert", GL_VERTEX_SHADER);
//    HDR->attachShader("Atmo/HDRRect.frag", GL_FRAGMENT_SHADER); //HDRSquare
//    HDR->init();




    Material * earth = new Simple("Color/debug");
    earth->addTexture(earthMap);

    camera = SceneData::Instance().getCurrentCamera();
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
    spacePlane->addBuffer(vertices, 3, "in_Normal");
    spacePlane->addBuffer(uvCoords, 2, "in_Uv");
    spacePlane->addElementBuffer(indicies);
    spacePlane->setDrawType(GL_TRIANGLES);
    Mesh * sphere = Geometry::gluSphere(m_fInnerRadius, 100, 50);

    spaceNode = new Node("space", { 0, 0, 0 }, 1, spacePlane, spaceFromAtmosphere);
    groundNode = new Node("sphere", { 0, 0, 0 }, 1, sphere, groundFromAtmosphere);
    groundNode->setRotation(QVector3D(-90, 0, 180));


  }
  void renderFrame(){
    RenderEngine::Instance().clear();

    bool drawSpace = false;
    if(camera->position.length() < m_fOuterRadius) {
      spaceNode->setMaterial(spaceFromAtmosphere);
      drawSpace = true;
    }else if(camera->position.z() > 0.0f) {
      spaceNode->setMaterial(spaceFromSpace);
      drawSpace = true;
    }


    if(drawSpace) {
      ShaderProgram * spaceShader = spaceNode->getMaterial()->getShaderProgram();
      spaceShader->use();
      spaceShader->setUniform("v3CameraPos", camera->position);
      spaceShader->setUniform("v3LightPos", lightDirection);
      spaceShader->setUniform("v3InvWavelength", QVector3D(1/m_fWavelength4[0], 1/m_fWavelength4[1], 1/m_fWavelength4[2]));
      spaceShader->setUniform("fCameraHeight", (float)camera->position.length());
      spaceShader->setUniform("fCameraHeight2", float(camera->position.length()*camera->position.length()));
      spaceShader->setUniform("fInnerRadius", m_fInnerRadius);
      spaceShader->setUniform("fInnerRadius2", m_fInnerRadius*m_fInnerRadius);
      spaceShader->setUniform("fOuterRadius", m_fOuterRadius);
      spaceShader->setUniform("fOuterRadius2", m_fOuterRadius*m_fOuterRadius);
      spaceShader->setUniform("fKrESun", m_Kr*m_ESun);
      spaceShader->setUniform("fKmESun", m_Km*m_ESun);
      spaceShader->setUniform("fKr4PI", m_Kr4PI);
      spaceShader->setUniform("fKm4PI", m_Km4PI);
      spaceShader->setUniform("fScale", 1.0f / (m_fOuterRadius - m_fInnerRadius));
      spaceShader->setUniform("fScaleDepth", m_fRayleighScaleDepth);
      spaceShader->setUniform("fScaleOverScaleDepth", (1.0f / (m_fOuterRadius - m_fInnerRadius)) / m_fRayleighScaleDepth);
      spaceShader->setUniform("g", m_g);
      spaceShader->setUniform("g2", m_g*m_g);
//      spaceShader->setUniform("s2Test", 0);
      spaceNode->setView(camera);
      spaceNode->draw();
    }


    if(camera->position.length() >= m_fOuterRadius)
      groundNode->setMaterial(groundFromSpace);
    else
      groundNode->setMaterial(groundFromAtmosphere);

    ShaderProgram * groundShader = groundNode->getMaterial()->getShaderProgram();
    groundShader->use();
    groundShader->setUniform("v3CameraPos", camera->position);
    groundShader->setUniform("v3LightPos", lightDirection);
    groundShader->setUniform("v3InvWavelength", QVector3D(1/m_fWavelength4[0], 1/m_fWavelength4[1], 1/m_fWavelength4[2]));
    groundShader->setUniform("fCameraHeight", (float)camera->position.length());
    groundShader->setUniform("fCameraHeight2", float(camera->position.length()*camera->position.length()));
    groundShader->setUniform("fInnerRadius", m_fInnerRadius);
    groundShader->setUniform("fInnerRadius2", m_fInnerRadius*m_fInnerRadius);
    groundShader->setUniform("fOuterRadius", m_fOuterRadius);
    groundShader->setUniform("fOuterRadius2", m_fOuterRadius*m_fOuterRadius);
    groundShader->setUniform("fKrESun", m_Kr*m_ESun);
    groundShader->setUniform("fKmESun", m_Km*m_ESun);
    groundShader->setUniform("fKr4PI", m_Kr4PI);
    groundShader->setUniform("fKm4PI", m_Km4PI);
    groundShader->setUniform("fScale", 1.0f / (m_fOuterRadius - m_fInnerRadius));
    groundShader->setUniform("fScaleDepth", m_fRayleighScaleDepth);
    groundShader->setUniform("fScaleOverScaleDepth", (1.0f / (m_fOuterRadius - m_fInnerRadius)) / m_fRayleighScaleDepth);
    groundShader->setUniform("g", m_g);
    groundShader->setUniform("g2", m_g*m_g);

    groundNode->setView(camera);
    groundNode->draw();


    GUI::Instance().draw();
    glError;
  }
};


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp("atmo").run();
}

