#ifndef MANDELBULB_H
#define MANDELBULB_H

#include <vector>
#include <QVector3D>
#include <QColor>
#include "Renderer/OpenGL.h"

using std::vector;

namespace fractal {

class MandelBulb {

public:
    static QVector3D triplexPow(QVector3D vec, unsigned n);

    static double norm(QVector3D in);
    static double abs(QVector3D in);

    static QVector3D triplexMult(QVector3D vec1, QVector3D vec2) ;

    static double distanceEstimate(QVector3D c, unsigned n, unsigned maxIter);

    static int mandelbulb(QVector3D c, int max);

    static void setColor(GLubyte *voxels, int sizex4, int size2x4,
                         int x, int y, int z,
                         GLubyte r, GLubyte g, GLubyte b, GLubyte a);
    static void generateVoxels(unsigned fromVoxel, unsigned toVoxel, vector<GLubyte> *voxels, int id, int size);

    static void setColor(vector<GLubyte> *voxels, GLubyte r, GLubyte g, GLubyte b, GLubyte a);

};


}

#endif // MANDELBULB_H
