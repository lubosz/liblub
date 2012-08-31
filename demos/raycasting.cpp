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
#include "raycasting.h"
#include "System/Application.h"
#include "System/Logger.h"
#include "Material/Shaders.h"
#include "System/TemplateEngine.h"
#include "Renderer/RenderPasses.h"
#include "Material/Textures.h"
#include "Mesh/Geometry.h"
#include "Scene/SceneGraph.h"
#include "Mesh/MandelBulb.h"
#include "Scene/Scene.h"

#include <thread>

using fractal::MandelBulb;

RaycastingDemo::RaycastingDemo() : Demo("raycasting") {
}

RaycastingDemo::~RaycastingDemo() {
    delete raycastingShader;
    delete backfaceFbo;
    delete fullPlane;
}

Texture* RaycastingDemo::createVolumeTexture(unsigned size) {
    GLubyte *voxels = new GLubyte[size*size*size* 4];

    unsigned sizex4 = size * 4;
    unsigned size2x4 = size * size * 4;

    unsigned u24 = size * 24.0f / 128.0f;
    unsigned u42 = size * 42.0f / 128.0f;

    unsigned u10 = size * 10.0f / 128.0f;
    unsigned u20 = size * 20.0f / 128.0f;
    unsigned u30 = size * 30.0f / 128.0f;
    unsigned u40 = size * 40.0f / 128.0f;
    unsigned u50 = size * 50.0f / 128.0f;
    unsigned u70 = size * 70.0f / 128.0f;
    unsigned u80 = size * 80.0f / 128.0f;
    unsigned u100 = size * 100.0f / 128.0f;

    float relSize = size / 128.0f;


    QVector3D foo = QVector3D(size - u20, size - u30, size - u30);
    QVector3D bar = QVector3D(u24,u24,u24);
    QVector3D center = QVector3D(size/2,size/2,size/2);

    for(unsigned x = 0; x < size; x++) {
        if(x%10 == 0)
            LogDebug << float(x)*100/float(size)<<"%";
        for(unsigned y = 0; y < size; y++) {
            for(unsigned z = 0; z < size; z++) {

                MandelBulb::setColor(voxels, sizex4, size2x4, x, y, z, int(z/relSize) % 250, int(y/relSize) % 250, 250, 230);

                QVector3D p = QVector3D(x, y, z) - foo;
                bool test = (p.length() < u42);
                if(test)
                    voxels[(x*4)+3 + (y * sizex4) + (z * size2x4)] = 0;

                p =	QVector3D(x,y,z)- center;
                test = (p.length() < u24);
                if(test)
                    voxels[(x*4)+3 + (y * sizex4) + (z * size2x4)] = 0;


                if(x > u20 && x < u40 && y > 0 && y < size && z > u10 &&  z < u50) {
                    MandelBulb::setColor(voxels, sizex4, size2x4, x, y, z, 100, 250, int(y/relSize)%100, 250);
                }

                if(x > u50 && x < u70 && y > 0 && y < size && z > u10 &&  z < u50) {
                    MandelBulb::setColor(voxels, sizex4, size2x4, x, y, z,250,250,int(y/relSize)%100,250);
                }

                if(x > u80 && x < u100 && y > 0 && y < size && z > u10 &&  z < u50) {
                    MandelBulb::setColor(voxels, sizex4, size2x4, x, y, z,250,70,int(y/relSize)%100,250);
                }

                p =	QVector3D(x,y,z)- bar;
                test = (p.length() < u40);
                if(test)
                    voxels[(x*4)+3 + (y * sizex4) + (z * size2x4)] = 0;
            }
        }
    }
    Texture* volumeTexture = new Texture3D("volume", voxels, size);
    LogDebug << "volume texture created";
    delete []voxels;
    return volumeTexture;
}

Texture * RaycastingDemo::ballGradient(int size) {
    GLubyte *voxels = new GLubyte[size*size*size*4];

    QVector3D center = QVector3D(size/2.0,size/2.0,size/2.0);
    unsigned sizex4 = size * 4;
    unsigned size2x4 = size * size * 4;

    for(int x = 0; x < size; x++) {
        if (x%10 == 0)
            LogDebug << float(x)*100/float(size)<<"%";
        for(int y = 0; y < size; y++) {
            for(int z = 0; z < size; z++) {

                QVector3D dist = QVector3D(x,y,z) - center;
                float fac = 1.0 - (dist.length() / static_cast<float>(size));
                GLubyte color = fac*255;

                voxels[(x*4) + (y * sizex4) + (z * size2x4)] = color;
                voxels[(x*4)+1 + (y * sizex4) + (z * size2x4)] = color;
                voxels[(x*4)+2 + (y * sizex4) + (z * size2x4)] = color;
                voxels[(x*4)+3 + (y * sizex4) + (z * size2x4)] = color;
            }
        }
    }
    Texture* volumeTexture = new Texture3D("volume", voxels, size);
    LogDebug << "volume texture created";
    delete []voxels;
    return volumeTexture;
}

Texture * RaycastingDemo::bulbThreads(int size) {

    unsigned threadCount = 8;
    unsigned threadSize = size / threadCount;

    vector<GLubyte> voxels;
    vector<vector<GLubyte>*> voxelParts;

    std::vector<std::thread> threads;

    for(unsigned i = 0; i < threadCount; i++){
        vector<GLubyte> *voxelPart = new vector<GLubyte>();
        voxelParts.push_back(voxelPart);

        threads.push_back(
                    std::thread(
                        MandelBulb::generateVoxels,
                        threadSize * i, threadSize * (i+1), voxelPart, i+1, size));
    }

    for(auto& thread : threads){
        thread.join();
    }

    for (unsigned i = 0; i < threadCount; i++) {
        foreach (GLubyte voxel, *voxelParts.at(i)) {
            voxels.push_back(voxel);
        }
    }

    LogDebug << "Sizes" << size*size*size*4 << voxels.size();

    Texture* volumeTexture = new Texture3D("volume", voxels.data(), size);
    LogDebug << "volume texture created";
    return volumeTexture;

}

void RaycastingDemo::init() {
    QList<string> attributes = QList<string> () << "uv";


    glEnable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0);

    res = Scene::Instance().getResolution();
    Texture* backfaceTexture = new ColorTexture(res, "backfaces");
    Texture* frontfaceTexture = new ColorTexture(res, "frontfaces");
    Texture* volumeTexture = bulbThreads(128);
    //        volumeTexture->filterMinMag();
    // Texture* volumeTexture = createVolumeTexture(512);
    //        Texture* volumeTexture = ballGradient(512);

    raycastingShader = new VertFragProgram("Stuff/raycasting",attributes);
    raycastingShader->addTexture(backfaceTexture);
    raycastingShader->addTexture(frontfaceTexture);
    raycastingShader->addTexture(volumeTexture);
    raycastingShader->samplerUniforms();

    raycastingShader->use();
    //        raycastingShader->setUniform("stepsize", .001);
    raycastingShader->setUniform("stepsize", 1.0/500.0);

    positionShader = new VertFragProgram("Color/position",QList<string> ());


    Scene::Instance().name = "Raycasting";

    backfaceFbo = new FrameBuffer(res);
    backfaceFbo->attachTexture(backfaceTexture);
    backfaceFbo->check();

    frontfaceFbo = new FrameBuffer(res);
    frontfaceFbo->attachTexture(frontfaceTexture);
    frontfaceFbo->check();

    fullPlane = Geometry::plane(QList<string> () << "uv", QRectF(-1, -1, 2, 2));

    //        Mesh * cube = Geometry::cube(QList<string> ());

    Mesh * cube = new Mesh(QList<string> ());
    cube->buffers["position"] = {
        1.0, -1.0, -1.0,
        1.0, -1.0, 1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0
    };
    cube->indices = {
        4, 0, 3, 4, 3, 7,
        1, 5, 2, 5, 6, 2,
        2, 6, 7, 2, 7, 3,
        0, 4, 1, 4, 5, 1,
        4, 7, 5, 7, 6, 5,
        0, 1, 2, 0, 2, 3
    };
    cube->init();
    cube->setDrawType(GL_TRIANGLES);

    Node * cubeNode = new Node("cubenode", QVector3D(-0.5,-0.75,-2), 1, cube, positionShader);
    SceneGraph::Instance().addNode(cubeNode);

    //        Scene::Instance().getCurrentCamera()->setParams(75, 0.01, 100);
    //        Scene::Instance().getCurrentCamera()->update();
    //        updatePerspective();

}

void RaycastingDemo::draw(){
    backfaceFbo->bind();
    OpenGL::Instance().clear();

    glCullFace(GL_FRONT);
    SceneGraph::Instance().drawNodes();
    backfaceFbo->unBind();

    frontfaceFbo->bind();
    OpenGL::Instance().clear();

    glCullFace(GL_BACK);
    SceneGraph::Instance().drawNodes();
    frontfaceFbo->unBind();

    OpenGL::Instance().clear();
    DrawThing::drawOnPlane(raycastingShader, fullPlane);
}

