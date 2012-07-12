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
#include <complex>

using std::complex;

class JuliaSet3D: public Application {
public:

    Mesh * mesh;
    ShaderProgram * shader;
    Node * node;
    FractalMesh * fractal;
    float phase;
    QList<string> attributes;

    JuliaSet3D(int argc, char *argv[]) :
        Application(argc, argv) {
        phase = 1;
        attributes = QList<string>() << "normal" << "color" << "uv";
        //            attributes = QList<string>() << "normal" << "uv";
    }

    ~JuliaSet3D() {
        delete node;
    }

    Mesh * plane(const QList<string> & attributes, unsigned resolution, float density) {
        Mesh * mesh = new Mesh(attributes);

        for(unsigned iY=0; iY < resolution; iY++) {
            for(unsigned iX=0; iX < resolution; iX++) {    /* initial value of orbit Z0 */

                QVector3D position = QVector3D(iX * density, iY * density, 0);

                mesh->vertex("position", position);
                //            mesh->vertex("color",1,0,0);
            }
        }

        mesh->calculateNormals(resolution);
        mesh->calculatePlaneIndex(resolution);

        mesh->init();
        mesh->setDrawType(GL_TRIANGLE_STRIP);
        return mesh;
    }

    Mesh * sinePlane(const QList<string> & attributes, unsigned resolution, float density, float phase) {
        Mesh * mesh = new Mesh(attributes);

        int res2 = resolution / 2;
        for(int iY=-res2; iY < res2; iY++) {
            for(int iX=-res2; iX < res2; iX++) {
                float x = iX * density;
                float y = iY * density;
                QVector2D fromCenter = QVector2D(x, y);
                mesh->vertex("position", x,1.0/fromCenter.length() * 10.0 * cos(sqrt(x*x+y*y) + phase), y );

                mesh->vertex("uv", static_cast<float>(iX)/res2, static_cast<float>(iY)/res2);
                //            mesh->vertex("color",1,0,0);
            }
        }

        mesh->calculatePlaneIndex(resolution);
        mesh->calculateNormals(resolution);

        mesh->init();
        mesh->setDrawType(GL_TRIANGLE_STRIP);
        return mesh;
    }

    void setOceanUniforms(ShaderProgram * program) {
        program->use();
        program->setUniform("deepColor", QVector4D(0, 0.3, 0.5, 1.0));
        program->setUniform("shallowColor",QVector4D(0, 1, 1, 1.0));
        program->setUniform("reflectionColor",QVector4D(0.95, 1, 1, 1.0));
        program->setUniform("reflectionAmount",1.0f);
        program->setUniform("reflectionBlur",0.0f);
        program->setUniform("waterAmount",0.3f);
        program->setUniform("fresnelPower",5.0f);
        program->setUniform("fresnelBias",0.328f);
        program->setUniform("hdrMultiplier", 0.471f);
        program->setUniform("textureScale",QVector2D(25, 26));
        program->setUniform("bumpSpeed",QVector2D(0.015, 0.005));
        program->setUniform("BumpScale",0.2f);
        program->setUniform("time",0.7f);
        program->setUniform("waveFreq",0.028f);
        program->setUniform("waveAmp",1.8f);
    }

    void scene() {

        ShaderProgram * ocean = new TemplateProgram("julia", attributes);



        //      ShaderProgram * ocean = new TemplateProgram("Ocean-small", attributes);
        //      setOceanUniforms(ocean);

        //      Texture * oceanNormal = new TextureFile("ocean/waves.png","NormalMap");
        //      ocean->addTexture(oceanNormal);
        //      Texture * oceanSky = new CubeTextureFile("cubemaps/sky","EnvironmentMap");
        //      ocean->addTexture(oceanSky);

        //      ocean->samplerUniforms();

        //    mesh = sinePlane(attributes, 100 , 1, phase);
        fractal = new FractalMesh(attributes, 1000 , .01, 2);
        //fractal = new FractalMesh(attributes, 400 , .01, 0.5);
        mesh = plane(attributes, 10, 1);

        fractal->setRange(3.0);



        node = new Node("Test", QVector3D(-5,-5,-5), 1, fractal, ocean);
        SceneGraph::Instance().addNode(node);

        //    QList<string> NormalAttributes = QList<string>() << "color";
        //    shader = new SimpleProgram("Particle/stars",NormalAttributes);
        //    Mesh * normalMesh = makeNormalMesh(mesh);
        //    Node * normalNode = new Node("Normals", QVector3D(-5,-5,-5), 1, normalMesh, shader);
        //    LogInfo << normalNode->getName();
        //    SceneGraph::Instance().addNode(normalNode);

    }

    void renderFrame() {
              fractal->regenerate();
        //      delete mesh;
        //      mesh = sinePlane(attributes, 100, 1, phase);
        //      mesh = julia(attributes, 400 , .1, phase);
        //      phase += 0.1;
              phase -= 0.001;
//              fractal->setRange(phase);

              fractal->C = complex<double>(phase,0.156);
//              fractal->C = complex<double>(0,0);
        OnePass::draw();
    }
};

int main(int argc, char *argv[]) {
    JuliaSet3D(argc, argv).run();
}

