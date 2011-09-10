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
#include "sponge.h"
#include "Mesh/MengerSponge.h"
#include "Scene/SceneData.h"
#include "Mesh/Geometry.h"
#include "Material/Textures.h"

Sponge::Sponge(int argc, char *argv[]) :
  Application(argc, argv) {
  SceneData::Instance().name = "Sponge";
  attributes = QList<string> () << "uv" << "normal" << "tangent";
}

void Sponge::initPasses() {
    Light * light = new Light(QVector3D(-9, 32, 11.5), QVector3D(1,-5,-1));
    SceneData::Instance().addLight("foolight", light);
    QSize res = SceneData::Instance().getResolution();
    vector<Texture*> shadowTargets = { new ShadowTexture(res, "shadowMap") };
    ShadowCastPass * shadowCastPass = new ShadowCastPass(res, shadowTargets, light);
    drawPasses.push_back(shadowCastPass);

    vector<Texture*> shadowReceiveTargets = {
            new ColorTexture(res, "finalColor")
    };

    vector<Texture*> shadowReceiveSources = {
            shadowCastPass->getTarget("shadowMap")
    };

    initMaterial();

    SourcePass * shadowReceivePass = new ShadowReceivePass(res,
            shadowReceiveSources, shadowReceiveTargets, material);
    drawPasses.push_back(shadowReceivePass);

    SinkPass * sinkPass = new SinkPass();
    sinkPass->debugTarget(QRectF(-1, -1, 2, 2),
            shadowReceivePass->getTarget("finalColor")
//            shadowCastPass->getTarget("shadowMap")
    );
    drawPasses.push_back(sinkPass);
}

void Sponge::initMaterial() {
    material = new EmptyMat();
    material->init();
    vector < string > flags = {
        "receiveShadows",
        "useSpotLight",
        "usePCF"
    };

    ShaderProgram * shaderProgram = material->getShaderProgram();
    shaderProgram->attachVertFrag("Color/PhongColor", flags);
    material->done(attributes);
    glError;
    shaderProgram->setUniform("ambientSceneColor",QVector4D(0.1, 0.1,0.1, 1.0));
    glError;
    shaderProgram->setUniform("diffuseMaterialColor",QVector4D(1,1,1,1));
  shaderProgram->setUniform("specularMaterialColor",
            QVector4D(0.8, 0.8, 0.8, 1.0));
  shaderProgram->setUniform("shininess",4.3f);
  shaderProgram->setUniform("yPixelOffset",1.0f/1200);
  shaderProgram->setUniform("xPixelOffset",1.0f/1920);

  shaderProgram->setUniform("lightColor",QVector4D(1.0, 1.0, 1.0, 1.0));
  // attenuation
  shaderProgram->setUniform("constantAttenuation",0.0f);
  shaderProgram->setUniform("linearAttenuation",0.1f);
  shaderProgram->setUniform("quadraticAttenuation",.005f);

  // spot
  shaderProgram->setUniform("spotOuterAngle",0.9f);
  shaderProgram->setUniform("spotInnerAngle",0.8f);
  SceneData::Instance().getLight("foolight")->bindShaderUpdateLight(material->getShaderProgram());
}

void Sponge::scene() {
    initPasses();
    QList<string> attributes = QList<string> () << "uv" << "normal" << "tangent";
    for (int i = 0; i < 5; i++) {
      MengerSponge * sponge = new MengerSponge(attributes, i);
      Node * node = new Node("sponge", { static_cast<float> (i - 2.5),
          static_cast<float> (i * 3 - 6), -5 }, 1, sponge->getMesh(), material);
      SceneGraph::Instance().addNode(node);
    }

    Node * plane = new Node("Plane", { 0, -30, 0 }, 20, Geometry::plane(attributes, QRectF(-1,-1,2,2)), material);
    plane->setRotation(QVector3D(90,0,0));
    plane->setReceiveShadows(true);
    plane->setCastShadows(false);
    SceneGraph::Instance().addNode(plane);
}

void Sponge::renderFrame() {
    foreach(DrawThing * pass, drawPasses)
        pass->draw();
}

int main(int argc, char *argv[]) {
  Sponge(argc, argv).run();
}
