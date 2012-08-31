#ifndef FRACTALMESH_H
#define FRACTALMESH_H

#include <complex>
#include <string>

#include <QList>
#include "Mesh/Mesh.h"

using std::complex;
using std::string;

class FractalMesh : public Mesh {
public:

    complex<double> C;

    double ZxMin;
    double ZxMax;
    double ZyMin;
    double ZyMax;

    double Pixelresolution;
    double PixelHeight;

    int IterationMax;
    /* bail-out value , radius of circle ;  */
    int EscapeRadius;

    complex<double> distance;

    unsigned resolution;

    float density;

    unsigned threadCount;
    unsigned step;

    FractalMesh(const QList<string> & attributes, unsigned resolution, float density, double range);
    static complex<double> f(complex<double> z, complex<double> c);
    int GiveLastIteration(complex<double> C,complex<double> Z);
    static int GiveLastIteration(complex<double> C,complex<double> Z, complex<double> * distance, unsigned IterationMax, unsigned EscapeRadius);
    void setRange(double range);
    void generate(unsigned from, unsigned to, vector<GLfloat> *positions, vector<GLfloat> *colors);
    void regenerate();
};

#endif // FRACTALMESH_H
