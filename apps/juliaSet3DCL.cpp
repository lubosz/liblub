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
#include "Scene/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Renderer/RenderPasses.h"
#include "System/Application.h"
#include "Material/Shaders.h"
#include <QVariantAnimation>
#include "Mesh/Geometry.h"
#include "Material/Textures.h"
#include "System/TemplateEngine.h"
#include "Mesh/FractalMesh.h"
#include "Renderer/OpenCL/OpenCL.h"
#include "Renderer/OpenCL/renderconfig.h"
#include <complex>

using std::complex;

class JuliaSet3DCL: public Application {
public:

    ShaderProgram * shader;
    QList<string> attributes;
    OpenCL *opencl;
    Mesh * fullPlane;
    Texture* renderResult;

    CLKernel *juliaKernel;
    CLBuffer *renderBuffer;
    CLBuffer *configBuffer;
    float *pixels;
    RenderingConfig config;

    JuliaSet3DCL(int argc, char *argv[]) :
        Application(argc, argv) {
        attributes = QList<string>() << "uv";
        opencl = new OpenCL();
        juliaKernel = new CLKernel("src/Renderer/OpenCL/rendering_kernel.cl");
    }

    ~JuliaSet3DCL() {
    }

    void freeBuffers() {
        delete renderBuffer;
        delete configBuffer;
        delete pixels;
    }

    void allocateBuffers() {
        const int colorCount = 3 * config.width * config.height;
        pixels = new float[colorCount];

        renderBuffer = new CLBuffer(&opencl->context, sizeof(float) * colorCount, pixels);
        configBuffer = new CLBuffer(&opencl->context, sizeof(RenderingConfig), &config);
    }

    void scene() {

        fullPlane = Geometry::plane(attributes, QRectF(-1, -1, 2, 2));
        shader = new SimpleProgram("Texture/texture", attributes);
        QSize size = QSize(1920,1200);
        renderResult = new ColorTexture(size, "diffuse");
        shader->addTexture(renderResult);
        shader->samplerUniforms();

        config.width = size.width();
        config.height = size.height();
        config.enableShadow = 1;
        config.superSamplingSize = 2;

        config.actvateFastRendering = 1;
        config.maxIterations = 8;
        config.epsilon = 0.003f * 0.75f;

        config.light[0] = 5.f;
        config.light[1] = 10.f;
        config.light[2] = 15.f;

        config.mu[0] = -0.2f;
        config.mu[1] = 0.4f;
        config.mu[2] = -0.4f;
        config.mu[3] = -0.4f;

        opencl->init();

        allocateBuffers();

        /* Create the kernel program */
        juliaKernel->createProgram(&opencl->context);
        juliaKernel->buildProgram(opencl->devices);
        juliaKernel->createKernel(opencl->devices, "JuliaGPU");

        Camera * cam = Scene::Instance().getCurrentCamera();
        cam->setPosition(QVector3D(1,2,8));
        cam->setDirection(-QVector3D(1,2,8));

        updateCamera();
        configBuffer->enqueueWrite(&opencl->commandQueue);
    }

    void renderFrame() {
        glError;
        freeBuffers();
        updateCamera();
        allocateBuffers();

        juliaKernel->setArg(0, sizeof(cl_mem), &renderBuffer->memory);
        juliaKernel->setArg(1, sizeof(cl_mem), &configBuffer->memory);

        /* Enqueue a kernel run call */
        size_t globalThreads[1];
        globalThreads[0] = config.width * config.height;
        if (globalThreads[0] % juliaKernel->workGroupSize != 0)
            globalThreads[0] = (globalThreads[0] / juliaKernel->workGroupSize + 1) * juliaKernel->workGroupSize;
        size_t localThreads[1];
        localThreads[0] = juliaKernel->workGroupSize;

        juliaKernel->setAccumulationArg(0, 0.f, 0.f);
        juliaKernel->enqueueRangeKernel(globalThreads, localThreads, &opencl->commandQueue);

        renderBuffer->enqueueRead(&opencl->commandQueue);

        renderResult->bind();
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB,
                     config.width,config.height,0,
                     GL_RGB,GL_FLOAT,pixels);
        OpenGL::Instance().clear();
        DrawThing::drawOnPlane(shader, fullPlane);
    }

    Vec vecConvert(const QVector3D &qvec) {
        Vec vec;
        vec.x = qvec.x();
        vec.y = qvec.y();
        vec.z = qvec.z();
        return vec;
    }

    void updateCamera() {
        Camera * cam = Scene::Instance().getCurrentCamera();

        QVector3D pos = cam->getPosition();
        QVector3D dir = cam->direction().normalized();
        QVector3D target = pos + dir;

        QVector3D up(0,1,0);
        QVector3D x = QVector3D::crossProduct(dir, up);
        x.normalize();

        x *= config.width * .5135f / config.height;

        QVector3D y = QVector3D::crossProduct(x, dir);
        y.normalize();
        y *= .5135f;

        config.camera.orig = vecConvert(pos);
        config.camera.dir = vecConvert(dir);
        config.camera.target = vecConvert(target);
        config.camera.y = vecConvert(y);
        config.camera.x = vecConvert(x);
    }

};

int main(int argc, char *argv[]) {
    JuliaSet3DCL(argc, argv).run();
}

