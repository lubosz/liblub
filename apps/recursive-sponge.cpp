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
#include "System/Application.h"
#include "Scene/TreeSponge.h"
#include "Material/Textures.h"
#include "Material/Shaders.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderPasses.h"
#include "Scene/SceneGraph.h"

class RecursiveSpongeApp: public Application {
public:
    ShaderProgram * shader;
    TreeSponge * sponge;
    Texture * texture;

    RecursiveSpongeApp(int argc, char *argv[]) : Application(argc,argv) {
    }

    ~RecursiveSpongeApp() {
        delete shader;
        delete texture;
    }

    void scene() {
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        QList<string> attributes;
        attributes.push_back("uv");

        shader = new TemplateProgram("instancing",attributes);

        texture = new TextureFile("diamond.png", "diffuse");
        texture->bind();
        texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
        shader->addTexture(texture);

        TreeSponge * sponge = new TreeSponge(5, attributes, shader);
        sponge->getMaterial()->addTexture(texture);

        SceneGraph::Instance().addNode(sponge);
    }

    void renderFrame(){
        OnePass::draw();
    }
};

int main(int argc, char *argv[]) {
    RecursiveSpongeApp(argc,argv).run();
}

