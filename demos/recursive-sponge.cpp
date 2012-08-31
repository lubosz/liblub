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
#include "recursive-sponge.h"
#include <string>
#include "System/Application.h"
#include "Scene/TreeSponge.h"
#include "Material/Textures.h"
#include "Material/Shaders.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderPasses.h"
#include "Scene/SceneGraph.h"

RecursiveSpongeApp::RecursiveSpongeApp() : Demo("recursive-sponge") {
}

RecursiveSpongeApp::~RecursiveSpongeApp() {
    delete shader;
    delete texture;
}

void RecursiveSpongeApp::init() {
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    QList<string> attributes;
    attributes.push_back("uv");

    shader = new VertFragProgram("Texture/transparentTexture",attributes);

    texture = new TextureFile("diamond.png", "diffuse");
    texture->bind();
    texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
    shader->addTexture(texture);

    TreeSponge * sponge = new TreeSponge(5, attributes, shader);
    sponge->getMaterial()->addTexture(texture);

    SceneGraph::Instance().addNode(sponge);
}

void RecursiveSpongeApp::draw(){
    OnePass::draw();
}

