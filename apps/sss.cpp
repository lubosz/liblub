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
#include "Mesh/MeshLoader.h"

class SSSApp: public Application {
public:

    SSSApp(int argc, char *argv[]) : Application(argc,argv) {
    }

    ~SSSApp() {
    }

    void scene() {
        QList<string> attributes = QList<string>() << "normal" << "tangent" << "bitangent"<< "uv";

//        QList<string> attributes = QList<string>() << "normal";

        ShaderProgram * shader;
        shader = new SimpleProgram("Stuff/sss", attributes);
//        shader = new SimpleProgram("Stuff/hemi_harmonic", attributes);
//        shader = new SimpleProgram("Stuff/brdf", attributes);


        Mesh * mesh = MeshLoader::load(attributes, "monkey.blend");
        Node * monkey = new Node("monkey", QVector3D(0,0,0), 1, mesh, shader);

        Texture * texture;
        texture = new TextureFile("diamond.png", "diffuseTexture");
        texture->bind();
        texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
        shader->addTexture(texture);

        SceneGraph::Instance().addNode(monkey);
    }

    void renderFrame(){
        OnePass::draw();
    }
};

int main(int argc, char *argv[]) {
    SSSApp(argc,argv).run();
}

