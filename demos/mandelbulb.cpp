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
#include "mandelbulb.h"
#include "Scene/SceneLoader.h"
#include "Scene/SceneGraph.h"
#include "Renderer/RenderPasses.h"
#include "System/Application.h"
#include "Material/Shaders.h"
#include <QVariantAnimation>

MandelbulbDemo::MandelbulbDemo() : Demo("mandelbulb") {
}

MandelbulbDemo::~MandelbulbDemo() {
}

double MandelbulbDemo::norm(QVector3D in) {
    return QVector3D::dotProduct(in, in);
}

QVector3D MandelbulbDemo::TriplexPow(QVector3D vec, unsigned n) {
    double z = vec.z();
    double r = sqrt(vec.x()*vec.x() + vec.y() * vec.y() + vec.z() * vec.z());
    double theta = 0;
    if (vec.x() != 0)
        theta = n * atan(vec.y()/vec.x());
    double phi = 0;
    if (r != 0)
        phi = n * asin(z/r);

    /*
      LogDebug << "z" << z
               << "r" << r
               << "theta" << theta
               << "phi" << phi;
*/
    return QVector3D(cos(theta)*cos(phi),
                     sin(theta)*sin(phi),
                     -sin(phi));
}
int MandelbulbDemo::Mandelbulb(QVector3D c) {

    QVector3D p = QVector3D(0, 0, 0);
    int i = 0;
    while(i < 24 && norm(p) < 4) {
        p = TriplexPow(p, 8) + c;
        i++;
        /*
          LogDebug << "i" << i
                   << "p" << p.x() << p.y() << p.z()
                   << "norm(p)" << norm(p);
                   */
    }
    return i;
}


Mesh * MandelbulbDemo::bulb(const QList<string> & attributes, float density) {
    Mesh * mesh = new Mesh(attributes);
    double n = 100;

    //        double from = -1.1;
    //        double to = 1.1;
    //        double step = 2.2/n;

    double from = -10;
    double to = 10;
    double step = 0.3;

    QMap<double, unsigned> colors;

    QVector3D position;

    for(double x = from; x < to; x+=step) {

        //            double progress;
        //            if (x < 0) {
        //                progress = fabs(from) - fabs(x);
        //            } else if (x >= 0) {
        //                progress = fabs(from) + x;
        //            }

        LogInfo << (x - from) * 100 / (fabs(from) + fabs(to)) << "%";

        for(double y = from; y < to; y+=step) {
            for(double z = from; z < to; z+=step) {

                double z2 = 1.1;
                //                  double x = 0;
                //                  double y = 0;

                position = QVector3D(x * density, y * density, z * density);
                while(z2 >= -0.1 && Mandelbulb(position) < 24) {
                    z2 -= 2.2/n;
                    //                      LogDebug << z2;
                }

                double color = fabs(z2);

                if (color > 1) {
                    mesh->vertex("position", position*10);
                    mesh->vertex("color", QVector3D(color, color, color));
                }
                //                    colors[color]++;

                /*
                    if (color != 0.11)
                        LogDebug << color;
                        */
            }
        }
    }

    /*
        foreach (double col, colors.keys())
            LogDebug << col << colors[col];
        */

    mesh->makeLinearIndex();

    mesh->init();
    mesh->setDrawType(GL_POINTS);
    return mesh;
}

void MandelbulbDemo::init() {
    QList<string> attributes = QList<string>() << "color";
    Mesh* mandelBulb = bulb(attributes, 0.1);

    ShaderProgram * stars = new VertFragProgram("Geometry/pointsize",attributes);

    Node * plane = new Node("Plane", QVector3D(-5,-10,-15), 1, mandelBulb, stars);
    SceneGraph::Instance().addNode(plane);

}
void MandelbulbDemo::draw() {
    OnePass::draw();
}

