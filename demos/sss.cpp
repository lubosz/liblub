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
#include "sss.h"
#include <string>
#include "System/Application.h"
#include "Procedural/TreeSponge.h"
#include "Texture/Textures.h"
#include "Shader/Shaders.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderPasses.h"
#include "Scene/SceneGraph.h"
#include "Load/MeshLoader.h"
#include "Scene/Scene.h"

SSSApp::SSSApp() : Demo("sss") {
}

SSSApp::~SSSApp() {
}

void SSSApp::init() {
    Scene::Instance().addCamera(QVector3D(1.31128, 0, 3.36602),
                                QVector3D(-0.309957, 0.0680153, -0.948315));

    QList<string> attributes = QList<string>() << "normal" << "tangent" << "bitangent"<< "uv";

    //        QList<string> attributes = QList<string>() << "normal";

    ShaderProgram * shader;
    shader = new VertFragProgram("Stuff/sss", attributes);
    //        shader = new VertFragProgram("Stuff/hemi_harmonic", attributes);
    //        shader = new VertFragProgram("Stuff/brdf", attributes);


    Mesh * mesh = MeshLoader::load(attributes, "monkey.blend");
    Node * monkey = new Node("monkey", QVector3D(0,0,0), 1, mesh, shader);

    Texture * texture;
    texture = new TextureFile("diamond.png", "diffuseTexture");
    texture->bind();
    texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
    shader->addTexture(texture);

    SceneGraph::Instance().addNode(monkey);
}

void SSSApp::draw(){
    OnePass::draw();
}

