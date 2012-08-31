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
#include "Scene/Scene.h"
#include "Mesh/Geometry.h"
#include "Material/Textures.h"
#include "Scene/SceneGraph.h"

Sponge::Sponge() : Demo("Sponge") {
    attributes = QList<string> () << "uv" << "normal" << "tangent";
}

void Sponge::initPasses() {
    Light * light = new Light(QVector3D(-9, 32, 11.5), QVector3D(1,-5,-1));
    Scene::Instance().addLight("foolight", light);
    QSize res = Scene::Instance().getResolution();
    vector<Texture*> shadowTargets = { new ShadowTexture(res, "shadowMap") };
    ShadowCastPass * shadowCastPass = new ShadowCastPass(res, shadowTargets, light);
    drawPasses.push_back(shadowCastPass);

    vector<Texture*> shadowReceiveTargets = {
        new ColorTexture(res, "finalColor")
    };

    vector<Texture*> shadowReceiveSources = {
        shadowCastPass->getTarget("shadowMap")
    };
    glError;
    initMaterial();
    glError;
    SourcePass * shadowReceivePass = new ShadowReceivePass(res,
                                                           shadowReceiveSources, shadowReceiveTargets, shader);
    drawPasses.push_back(shadowReceivePass);

    SinkPass * sinkPass = new SinkPass();
    sinkPass->debugTarget(QRectF(-1, -1, 2, 2),
                          shadowReceivePass->getTarget("finalColor")
                          //            shadowCastPass->getTarget("shadowMap")
                          );
    drawPasses.push_back(sinkPass);
}

void Sponge::initMaterial() {
    //    vector<string> flags = {
    //        "receiveShadows",
    //        "useSpotLight",
    //        "usePCF"
    //    };
    shader = new VertFragProgram("Shading/PhongColor", attributes);
    shader->setUniform("ambientSceneColor", QVector4D(0.1, 0.1,0.1, 1.0));
    shader->setUniform("diffuseMaterialColor", QVector4D(1,1,1,1));
    shader->setUniform("specularMaterialColor", QVector4D(0.8, 0.8, 0.8, 1.0));
    shader->setUniform("shininess", 4.3f);
    shader->setUniform("yPixelOffset", 1.0f/1200);
    shader->setUniform("xPixelOffset", 1.0f/1920);

    shader->setUniform("lightColor", QVector4D(1.0, 1.0, 1.0, 1.0));
    // attenuation
    shader->setUniform("constantAttenuation", 0.0f);
    shader->setUniform("linearAttenuation", 0.1f);
    shader->setUniform("quadraticAttenuation", .005f);

    // spot
    shader->setUniform("spotOuterAngle",0.9f);
    shader->setUniform("spotInnerAngle",0.8f);
    Scene::Instance().getLight("foolight")->bindShaderUpdateLight(shader);
}

void Sponge::init() {
    initPasses();
    QList<string> attributes = QList<string> () << "uv" << "normal" << "tangent";
    for (int i = 0; i < 5; i++) {
        MengerSponge * sponge = new MengerSponge(attributes, i);
        Node * node = new Node("sponge", QVector3D(i - 2.5, i * 3 - 6, -5), 1, sponge->getMesh(), shader);
        SceneGraph::Instance().addNode(node);
    }

    Node * plane = new Node("Plane", QVector3D(0, -30, 0), 20, Geometry::plane(attributes, QRectF(-1,-1,2,2)), shader);
    plane->setRotation(QVector3D(90,0,0));
    plane->setReceiveShadows(true);
    plane->setCastShadows(false);
    SceneGraph::Instance().addNode(plane);
}

void Sponge::draw() {
    foreach(DrawThing * pass, drawPasses)
        pass->draw();
}
