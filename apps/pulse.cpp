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
#include "Scene/Scene.h"
#include "System/Logger.h"
#include "Scene/TreeSponge.h"
#include "Mesh/Geometry.h"
#include "Material/Textures.h"
#include "Renderer/OpenGL.h"
#include "Renderer/FrameBuffer.h"
#include "Material/Shaders.h"
#include "Scene/Scene.h"
#include "Scene/InstancedSponge.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderPasses.h"
#include "Mesh/MengerSponge.h"
#include "Scene/SceneGraph.h"
#include "Renderer/DeferredRenderer.h"

class Ball : public Node {
public:

    Ball() {
        position = QVector3D(0,0,0);
    }

    QVector3D makeVectorFromPolarCoords(float amount, float length) {
        QVector3D returnMe = QVector3D(length, -length/10.0, 0);
        QMatrix4x4 rotation = QMatrix4x4();
        rotation.setToIdentity();
        rotation.rotate(amount, QVector3D(0, 1, 0));
        return rotation * returnMe;
    }
};

class PulseApp: public Application {
public:

    float rotation;

    float pulse;
    vector<Ball*> balls;


    PulseApp(int argc, char *argv[]) : Application(argc,argv) {
        pulse = 0;
        rotation = 0;
    }

    ~PulseApp() {
//        delete texture;
//        for(auto ball : balls)
//            delete ball;
    }

    void scene() {
        Scene::Instance().getCurrentCamera()->setPosition(QVector3D(-1.43765, 5.130675, -1.20157));
        Scene::Instance().getCurrentCamera()->setDirection(QVector3D(0.741701, -0.0836778, 0.66549));
        Scene::Instance().getCurrentCamera()->update();


        DeferredRenderer::Instance().initSky("cubemaps/sky");

        Mesh* mesh;

        QList<string> attributes = QList<string>() << "uv" << "normal" << "tangent" << "bitangent";
        Material * someMaterial = new Material("someMaterial");

//        Texture * diffuse = new TextureFile("Planets/Mars.jpg", "diffuseTexture");
//        Texture * normal = new TextureFile("Planets/Mars-normal.png", "normalTexture");
//        someMaterial->addTexture(diffuse);
//        someMaterial->addTexture(normal);

        mesh = Geometry::sphere(attributes, 1, 10, 10);

        for (int i = 0; i < 500; i++) {
            Ball * ball;
            ball = new Ball();
            ball->setMaterial(someMaterial);
            ball->setMesh(mesh);
            SceneGraph::Instance().addNode(ball);
            balls.push_back(ball);
        }

        DeferredRenderer::Instance().init();
    }

    void renderFrame(){
        pulse += .1;
        rotation = 0;
        for (auto romanesco : balls) {
            QVector3D theVector = romanesco->makeVectorFromPolarCoords(-10*rotation, rotation/10);
            romanesco->setSize((sin(pulse+rotation)+1.0)/10.0+rotation/100);
            romanesco->setDirection(theVector);
            romanesco->setPosition(theVector);
            romanesco->update();
            rotation += .1;

        }
//        OnePass::draw();
        DeferredRenderer::Instance().draw();
    }
};

int main(int argc, char *argv[]) {
    PulseApp app(argc,argv);
    app.run();
}


