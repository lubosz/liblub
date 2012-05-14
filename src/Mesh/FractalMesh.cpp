#include "FractalMesh.h"
#include <QList>
#include <QColor>

FractalMesh::FractalMesh(const QList<string> & attributes, unsigned resolution, float density, double range) : Mesh(attributes){

    //        C = complex<double>(0.285,0);
    //        C = complex<double>(-1.0,0.1);
    //        C = complex<double>(-0.4,0.6);
    C = complex<double>(-0.8,0.156);

    this->resolution = resolution;
    this->density = density;

    /* world ( double) coordinate = parameter plane*/
    //      const double range = 0.5;
    setRange(range);


    IterationMax = 500;
    /* bail-out value , radius of circle ;  */
    EscapeRadius = 10;

    distance = 0;

    generate();
}

complex<double> FractalMesh::f(complex<double> z, complex<double> c) {
    return c + z*z;
}

int FractalMesh::GiveLastIteration(complex<double> C,complex<double> Z) {
    complex<double> h;
    distance = 0;
    int i;
    for(i=0; i < IterationMax && abs(Z) <= EscapeRadius; i++) {
        h = Z;
        Z = f(Z, C);
        distance = distance + abs(Z - h);
    }
    return i;
}

void FractalMesh::setRange(double range) {
    ZxMin=-range;
    ZxMax=range;
    ZyMin=-range;
    ZyMax=range;
    Pixelresolution=(ZxMax-ZxMin)/resolution;
    PixelHeight=(ZyMax-ZyMin)/resolution;
}


void FractalMesh::generate() {

    for(unsigned iY=0; iY < resolution; ++iY) {

        LogInfo << iY * 100.0 / resolution<< "%";
        double Zimag = ZyMax - iY * PixelHeight; /* reverse Y  axis */

        if (fabs(Zimag) < PixelHeight / 2)
            Zimag = 0.0;

        for(unsigned iX=0; iX < resolution; ++iX) {    /* initial value of orbit Z0 */

            complex<double> Z = complex<double>(ZxMin + iX * Pixelresolution, Zimag);

            int Iteration = GiveLastIteration(C, Z);

            double measure = distance.real() / static_cast<double>(Iteration);
            QColor color(0,0,0);

            float measureMapped = 0;

            if (Iteration==IterationMax) {
                /*  interior of Filled-in Julia set  = black */
                measureMapped = (measure - 1.0) * 10;
            } else {
                /* exterior of Filled-in Julia set  = white */
                //                measureMapped = sqrt(measure / 10000.0);
                measureMapped = (measure - 10) / 10.0;
            }
            //              mesh->vertex("position", iX * density, measureMapped,iY * density);
            double theColor = static_cast<double>(Iteration) / static_cast<double>(IterationMax);
            vertex("position", iX * density, theColor,iY * density);

            float colorComponent = measureMapped * 50;
            if (colorComponent < 0)
                colorComponent = -colorComponent;
            //              color.setHsv ( colorComponent, 255, 255);
            //            LogDebug << measure << measureMapped << colorComponent;


            theColor *= 255.0;
            color.setRgb(theColor,theColor,theColor);

            vertex("color",
                         static_cast<GLfloat>(color.redF()),
                         static_cast<GLfloat>(color.greenF()),
                         static_cast<GLfloat>(color.blueF()));

            //              mesh->vertex("uv", static_cast<float>(iX)/resolution, static_cast<float>(iY)/resolution);
        }
    }

    calculatePlaneIndex(resolution);
    //       calculateNormals(mesh, resolution);

    init();
    //     mesh->setDrawType(GL_POINTS);
    setDrawType(GL_TRIANGLE_STRIP);
}


void FractalMesh::regenerate() {

    buffers["position"].clear();
    buffers["normal"].clear();

    vector<GLfloat> * colorBuffer = &buffers["color"];

    colorBuffer->clear();

    for(unsigned iY=0; iY < resolution; ++iY) {

        //         LogInfo << iY * 100.0 / resolution<< "%";
        double Zimag = ZyMax - iY * PixelHeight; /* reverse Y  axis */

        if (fabs(Zimag) < PixelHeight / 2)
            Zimag = 0.0;
        for(unsigned iX=0; iX < resolution; ++iX) {
            //initial value of orbit Z0

            complex<double> Z = complex<double>(ZxMin + iX * Pixelresolution, Zimag);
            int Iteration = GiveLastIteration(C, Z);
            //              int Iteration = 1;
            //              distance = 0;

            double measure = distance.real() / static_cast<double>(Iteration);
            QColor color(0,0,0);

            float measureMapped = 0;

            if (Iteration==IterationMax) {
                //  interior of Filled-in Julia set  = black
                measureMapped = (measure - 1.0) * 10;
            } else {
                // exterior of Filled-in Julia set  = white
                //                measureMapped = sqrt(measure / 10000.0);
                measureMapped = (measure - 10) / 10.0;
            }
            vertex("position", iX * density, measureMapped,iY * density);

            float colorComponent = measureMapped * 50;
            if (colorComponent < 0)
                colorComponent = -colorComponent;
            color.setHsv ( colorComponent, 255, 255);
            //            LogDebug << measure << measureMapped << colorComponent;

            //              mesh->vertex("color",
            //                           static_cast<GLfloat>(color.redF()),
            //                           static_cast<GLfloat>(color.greenF()),
            //                           static_cast<GLfloat>(color.blueF()));

            //              mesh->vertex("color",0,0,0);

            colorBuffer->push_back(color.redF());
            colorBuffer->push_back(color.greenF());
            colorBuffer->push_back(color.blueF());

        }
    }

    calculateNormals(resolution);

    //       mesh->updateBuffer("position");
    //       mesh->updateBuffer("normal");
    //       mesh->updateBuffer("color");

    meshBuffers["in_Vertex"]->write_dynamic(buffers["position"]);
    meshBuffers["in_Normal"]->write_dynamic(buffers["normal"]);
    meshBuffers["in_Color"]->write_dynamic(buffers["color"]);

}

