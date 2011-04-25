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
#include "Material/ShaderProgram.h"

class AtmosphereApp: public Application {
 public:
  Material *groundFromAtmosphere, *groundFromSpace,
                *skyFromAtmosphere, *skyFromSpace,
                *spaceFromAtmosphere,*spaceFromSpace,
                *HDR;
  Material *terrainMat;
  bool useHDR;
  float innerRadius;
  float outerRadius;
  Camera* camera;
  Light * light;
  Node * spaceNode,* groundNode,* skyNode, *terrainNode, *oceanNode;
  Material * ocean;

  FrameBuffer *fbo;
  Texture * targetTexture;

  AtmosphereApp() {
    useHDR = false;
    innerRadius = 11.0f;
    outerRadius = 11.55f;
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
    QVector3D lightDirection = lightPosition / lightPosition.length();

    float rayleigh = 0.0025f; // Rayleigh scattering constant
    float rayleigh4Pi = rayleigh * 4.0f * M_PI;
    float mie = 0.0010f; // Mie scattering constant
    float mie4Pi = mie * 4.0f * M_PI;
    float sun = 20.0f; // Sun brightness constant
    float g = -0.990f; // The Mie phase asymmetry factor
    float rayleighScaleDepth = 0.25f;
//    float mieScaleDepth = 0.1f;

    program->use();
    program->setUniform("lightPosition", lightDirection);
    program->setUniform("invWavelength", QVector3D(1 / wavelength4[0], 1 / wavelength4[1], 1 / wavelength4[2]));
    program->setUniform("innerRadius", innerRadius);
    program->setUniform("innerRadius2", innerRadius * innerRadius);
    program->setUniform("outerRadius", outerRadius);
    program->setUniform("outerRadius2", outerRadius * outerRadius);
    program->setUniform("rayleighSun", rayleigh * sun);
    program->setUniform("mieSun", mie * sun);
    program->setUniform("rayleigh4Pi", rayleigh4Pi);
    program->setUniform("mie4Pi", mie4Pi);
    program->setUniform("invSphereDistance", 1.0f / (outerRadius - innerRadius));
    program->setUniform("scaleDepth", rayleighScaleDepth);
    program->setUniform("scaleOverScaleDepth", (1.0f / (outerRadius - innerRadius)) / rayleighScaleDepth);
    program->setUniform("g", g);
    program->setUniform("g2", g * g);
  }
  void setCameraUniforms(ShaderProgram * program){
    program->use();
    program->setUniform("cameraPosition", camera->position);
    program->setUniform("cameraHeight", (float)camera->position.length());
    program->setUniform("cameraHeight2", float(camera->position.length()*camera->position.length()));
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
    moonPlane->addBuffer(vertices, 3, "in_Normal");
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
    attributes.push_back("normal");
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

    Mesh * innerSphere = Geometry::sphere(innerRadius, 100, 50);
    Mesh * outerSphere = Geometry::sphere(outerRadius, 300, 500);

//    Mesh * monkey = MeshFactory::load("monkey.obj");

    spaceNode = new Node("space", { 0, 0, 0 }, 1, moonPlane(), spaceFromAtmosphere);
    groundNode = new Node("ground", { 0, 0, 0 }, 1, innerSphere, groundFromAtmosphere);
    groundNode->setRotation(QVector3D(-90, 0, 0));
    skyNode = new Node("sky", { 0, 0, 0 }, 1, outerSphere, skyFromAtmosphere);

//    groundNode = new Node("ground", { 0, 0, 0 }, 10, monkey, groundFromAtmosphere);
////    groundNode->setRotation(QVector3D(-90, 0, 0));
//    skyNode = new Node("sky", { 0, 0, 0 }, 11, monkey, skyFromAtmosphere);

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

    terrainMat = new EmptyMat();

    terrainMat->init();
//    terrainMat->getShaderProgram()->attachShader("Atmo/GroundTesselation.vert",GL_VERTEX_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.vert",GL_VERTEX_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.eval",GL_TESS_EVALUATION_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.cont",GL_TESS_CONTROL_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.geom",GL_GEOMETRY_SHADER,true);
//    terrainMat->getShaderProgram()->attachShader("Atmo/GroundTesselation.frag",GL_FRAGMENT_SHADER,true);
    terrainMat->getShaderProgram()->attachShader("Tesselation/Tesselation.frag",GL_FRAGMENT_SHADER,true);
    Texture * groundTexture = TextureFactory::Instance().load("terrain/mud.jpg","diffuse");
    Texture * noise = TextureFactory::Instance().load("terrain-noise-blur.jpg","noise");
    terrainMat->addTexture(groundTexture);
    terrainMat->addTexture(noise);
    terrainMat->done(attributes);

    terrainMat->getShaderProgram()->setUniform("TessLevelInner",1.0f);
    terrainMat->getShaderProgram()->setUniform("TessLevelOuter",1.0f);
    terrainMat->getShaderProgram()->setUniform("LightPosition", QVector3D(0.25, 0.25, 1));
    setAtmoUniforms(terrainMat->getShaderProgram());
    Mesh * groundMesh = MeshLoader::load("earth.obj");
//    Mesh * mesh = Geometry::gluSphere(10.0f, 100, 50);
//    Mesh * mesh = Geometry::makeIcosahedron();
    groundMesh->setDrawType(GL_PATCHES);
    terrainNode = new Node("ground", { 0, 0, 0 }, 12.1, groundMesh, terrainMat);

    GUI::Instance().addText("tess", "Tess");
    GUI::Instance().addText("dist", "Dist");

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

    Texture * oceanNormal = TextureFactory::Instance().load("ocean/waves.png","NormalMap");
    ocean->addTexture(oceanNormal);
    Texture * oceanSky = TextureFactory::Instance().loadCubeMap("cubemaps/sky","EnvironmentMap");
    ocean->addTexture(oceanSky);

    oceanNode = new Node("ocean", { 0, 0, 0 }, 1, innerSphere, ocean);

  }
  void renderFrame(){

    updateTesselation();

    if(useHDR) {
      fbo->bind();
      fbo->updateRenderView();
    }
    RenderEngine::Instance().clear();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

//    drawMoon();
    drawGround();
    float time = float(Timer::Instance().secoundsPassed) + float(Timer::Instance().nanosecoundsPassed)/1000000000.0;
    oceanNode->setView(camera);
    ocean->getShaderProgram()->setUniform("time",time);
    ocean->getShaderProgram()->setUniform("eyePosition",camera->position);
    oceanNode->draw();

    drawSky();
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

  void updateTesselation(){
    while (camera->position.length() < 11.01){
      camera->position += camera->position.normalized() *  0.01;
      camera->update();
    }
    int maxTess = 60;
    float tessStartDistance = 8;
    float scale = maxTess - (camera->position.length() - tessStartDistance);

    std::stringstream tess;
    tess << "Tess " << int(scale);
    GUI::Instance().updateText("tess",tess.str());

    std::stringstream dist;
    dist << "Dist " << camera->position.length();
    GUI::Instance().updateText("dist",dist.str());

    if (scale > 1){
      terrainMat->getShaderProgram()->use();
      terrainMat->getShaderProgram()->setUniform("TessLevelInner",scale);
      terrainMat->getShaderProgram()->setUniform("TessLevelOuter",scale);
    }
  }

  void drawMoon(){
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
  }

  void drawGround(){
//        if(camera->position.length() >= outerRadius)
//          groundNode->setMaterial(groundFromSpace);
//        else
//          groundNode->setMaterial(groundFromAtmosphere);
//
//        setCameraUniforms(groundNode->getMaterial()->getShaderProgram());
//        groundNode->setView(camera);
//
//        groundNode->draw();

            setCameraUniforms(terrainNode->getMaterial()->getShaderProgram());
            terrainNode->setView(camera);
            terrainNode->draw();
  }

  void drawSky(){
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
  }
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  AtmosphereApp().run();
}

