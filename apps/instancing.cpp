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
#include "System/GUI.h"
#include "Scene/SceneLoader.h"
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "System/Timer.h"
#include "Scene/Node.h"
#include "Scene/TreeSponge.h"
#include "Mesh/Geometry.h"
#include "Material/Textures.h"
#include "Renderer/RenderEngine.h"
#include "Renderer/FrameBuffer.h"
#include "Scene/SceneLoader.h"
#include "Material/Shaders.h"
#include "Scene/Scene.h"
#include "Scene/InstancedSponge.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderPasses.h"
#include "Mesh/MengerSponge.h"

class InstancingApp: public Application {
 public:
  ShaderProgram * shader;

  InstancingApp(int argc, char *argv[]) : Application(argc,argv) {
  }

  ~InstancingApp() {}

  void scene() {
//      glEnable(GL_BLEND);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
      glDepthFunc(GL_LESS);
    QList<string> attributes;
//    attributes.push_back("normal");
    attributes.push_back("uv");
//
//    InstancedSponge *sponge = new InstancedSponge(5, attributes);
//    TemplateEngine::Instance().c.insert("positionElements", QVariant::fromValue(sponge->positionBufferDataSize));
//    TemplateEngine::Instance().c.insert("useInstancing", true);
    shader = new TemplateProgram("instancing",attributes);
//    sponge->initBuffers(material);

    Texture * texture = new TextureFile("diamond.png", "diffuse");
    texture->bind();
    texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
    shader->addTexture(texture);


//    MengerSponge * meshSponge = new MengerSponge(attributes, 5);
//    Node * sponge = new Node("sponge", { 0,0,0 }, 1, meshSponge->getMesh(), material);

    TreeSponge * sponge = new TreeSponge(5, attributes);
    sponge->setMaterial(shader);

    SceneGraph::Instance().addNode(sponge);
  }

  void renderFrame(){
      glEnable(GL_BLEND);
    OnePass::draw();
  }

  vector<QVector4D> * initPositionData(QVector3D translation) {
        vector<QVector4D> * positionBufferData = new vector<QVector4D> ();
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < 16; z++) {
                    positionBufferData->push_back(
                            QVector4D(3 * x, 3 * y, 3 * z, 1)
                                    + translation.toVector4D());
                }
            }
        }
        return positionBufferData;
    }
};

int main(int argc, char *argv[]) {
  InstancingApp app(argc,argv);
  app.run();
}

