#include "FractalMesh.h"
#include <QList>
#include <QColor>
#include <thread>

static void generate_static(unsigned from, unsigned to,
            vector<GLfloat> *positions, vector<GLfloat> *colors,
            unsigned resolution, unsigned IterationMax, unsigned EscapeRadius,
            double Pixelresolution, double PixelHeight,
            double ZyMax, double ZxMin,
            complex<double> C, float density) {
    complex<double> distance = 0;

    for(unsigned iY=from; iY < to; ++iY) {

//        LogInfo << iY * 100.0 / resolution<< "%";
        double Zimag = ZyMax - iY * PixelHeight; /* reverse Y  axis */

        if (fabs(Zimag) < PixelHeight / 2)
            Zimag = 0.0;

        for(unsigned iX=0; iX < resolution; ++iX) {    /* initial value of orbit Z0 */

            complex<double> Z = complex<double>(ZxMin + iX * Pixelresolution, Zimag);

            int Iteration = FractalMesh::GiveLastIteration(C, Z, &distance, IterationMax, EscapeRadius);

            double measure = distance.real() / static_cast<double>(Iteration);
            QColor color(0,0,0);

            float measureMapped = 0;

            if (Iteration==IterationMax) {
                measureMapped = (measure - 1.0) * 10;
            } else {
                measureMapped = (measure - 10) / 10.0;
            }
            double theColor = static_cast<double>(Iteration) / static_cast<double>(IterationMax);

            positions->push_back(iX * density);
//            positions->push_back(theColor);
            positions->push_back(0);
            positions->push_back(iY * density);


            float colorComponent = measureMapped * 50;
            if (colorComponent < 0)
                colorComponent = -colorComponent;

            theColor *= 255.0;
            color.setRgb(theColor,theColor,theColor);

            colors->push_back(static_cast<GLfloat>(color.redF()));
            colors->push_back(static_cast<GLfloat>(color.greenF()));
            colors->push_back(static_cast<GLfloat>(color.blueF()));
        }
    }
}

static void generate_static_noposition(unsigned from, unsigned to,
            vector<GLfloat> *colors,
            unsigned resolution, unsigned IterationMax, unsigned EscapeRadius,
            double Pixelresolution, double PixelHeight,
            double ZyMax, double ZxMin,
            complex<double> C, float density) {

    for(unsigned iY=from; iY < to; ++iY) {

        double Zimag = ZyMax - iY * PixelHeight;

        if (fabs(Zimag) < PixelHeight / 2)
            Zimag = 0.0;

        for(unsigned iX=0; iX < resolution; ++iX) {

            complex<double> Z = complex<double>(ZxMin + iX * Pixelresolution, Zimag);

//            int Iteration = FractalMesh::GiveLastIteration(C, Z, &distance, IterationMax, EscapeRadius);
            int Iteration;
            for(Iteration=0; Iteration < IterationMax && abs(Z) <= EscapeRadius; Iteration++) {
                Z = FractalMesh::f(Z, C);
            }
//            int Iteration = IterationMax;

            GLfloat theColor = static_cast<GLfloat>(Iteration) / static_cast<GLfloat>(IterationMax);

            colors->push_back(theColor);
            colors->push_back(theColor);
            colors->push_back(theColor);
        }
    }
}


FractalMesh::FractalMesh(const QList<string> & attributes, unsigned resolution, float density, double range) : Mesh(attributes){

    //        C = complex<double>(0.285,0);
    //        C = complex<double>(-1.0,0.1);
    //        C = complex<double>(-0.4,0.6);
//    C = complex<double>(-0.8,0.156);
    C = complex<double>(-0.8,0);

    threadCount = 50;

    resolution = resolution - (resolution % threadCount);

    step = resolution / threadCount;

    this->resolution = resolution;
    this->density = density;

    /* world ( double) coordinate = parameter plane*/
    //      const double range = 0.5;
    setRange(range);


    IterationMax = 50;
    /* bail-out value , radius of circle ;  */
    EscapeRadius = 10;

    distance = 0;

    vector<vector<GLfloat>*> positions;
    vector<vector<GLfloat>*> colors;

    std::vector<std::thread> threads;

    for(int i = 0; i < threadCount; ++i){
        vector<GLfloat> * pos = new vector<GLfloat>();
        positions.push_back(pos);
        vector<GLfloat> * col = new vector<GLfloat>();
        colors.push_back(col);

        threads.push_back(std::thread(generate_static, i*step, (i+1)*step,
                               pos, col,
                               resolution, IterationMax, EscapeRadius,
                               Pixelresolution, PixelHeight,
                               ZyMax, ZxMin,
                               C, density));

    }

    for(auto& thread : threads){
        thread.join();
    }

    for(auto* pos : positions){
        buffers["position"].insert(buffers["position"].end(), pos->begin(), pos->end());
        delete pos;
    }

    for(auto* col : colors){
        buffers["color"].insert(buffers["color"].end(), col->begin(), col->end());
        delete col;
    }

//    generate(0, resolution, &positions, &colors);


    calculatePlaneIndex(resolution);
    //       calculateNormals(mesh, resolution);

    init();
    //     mesh->setDrawType(GL_POINTS);
    setDrawType(GL_TRIANGLE_STRIP);
}

complex<double> FractalMesh::f(complex<double> z, complex<double> c) {
    return c *c + z*z*z*z*z;
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

int FractalMesh::GiveLastIteration(complex<double> C,complex<double> Z, complex<double> * distance, unsigned IterationMax, unsigned EscapeRadius) {
    complex<double> h;
    *distance = 0;
    int i;
    for(i=0; i < IterationMax && abs(Z) <= EscapeRadius; i++) {
        h = Z;
        Z = f(Z, C);
        *distance = *distance + abs(Z - h);
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


void FractalMesh::generate(unsigned from, unsigned to, vector<GLfloat> *positions, vector<GLfloat> *colors) {

    for(unsigned iY=from; iY < to; ++iY) {

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
//            vertex("position", iX * density, theColor,iY * density);

            positions->push_back(iX * density);
            positions->push_back(theColor);
            positions->push_back(iY * density);


            float colorComponent = measureMapped * 50;
            if (colorComponent < 0)
                colorComponent = -colorComponent;
            //              color.setHsv ( colorComponent, 255, 255);
            //            LogDebug << measure << measureMapped << colorComponent;


            theColor *= 255.0;
            color.setRgb(theColor,theColor,theColor);

//            vertex("color",
//                         static_cast<GLfloat>(color.redF()),
//                         static_cast<GLfloat>(color.greenF()),
//                         static_cast<GLfloat>(color.blueF()));

            colors->push_back(static_cast<GLfloat>(color.redF()));
            colors->push_back(static_cast<GLfloat>(color.greenF()));
            colors->push_back(static_cast<GLfloat>(color.blueF()));

            //              mesh->vertex("uv", static_cast<float>(iX)/resolution, static_cast<float>(iY)/resolution);
        }
    }


}


void FractalMesh::regenerate() {

//    buffers["position"].clear();
//    buffers["normal"].clear();

    vector<GLfloat> * colorBuffer = &buffers["color"];

    colorBuffer->clear();

//    vector<GLfloat> positions;
//    vector<GLfloat> colors;

//    generate(0, resolution, &positions, &colors);

//    buffers["position"].insert(buffers["position"].end(), positions.begin(), positions.end());
//    buffers["color"].insert(buffers["color"].end(), colors.begin(), colors.end());

//    vector<vector<GLfloat>*> positions;
    vector<vector<GLfloat>*> colors;

    std::vector<std::thread> threads;

    for(int i = 0; i < threadCount; ++i){
//        vector<GLfloat> * pos = new vector<GLfloat>();
//        positions.push_back(pos);
        vector<GLfloat> * col = new vector<GLfloat>();
        colors.push_back(col);

        threads.push_back(std::thread(generate_static_noposition, i*step, (i+1)*step,
                               col,
                               resolution, IterationMax, EscapeRadius,
                               Pixelresolution, PixelHeight,
                               ZyMax, ZxMin,
                               C, density));

    }

    for(auto& thread : threads){
        thread.join();
    }

//    for(auto* pos : positions){
//        buffers["position"].insert(buffers["position"].end(), pos->begin(), pos->end());
//        delete pos;
//    }

    for(auto* col : colors){
        buffers["color"].insert(buffers["color"].end(), col->begin(), col->end());
        delete col;
    }

//    calculateNormals(resolution);

    //       mesh->updateBuffer("position");
    //       mesh->updateBuffer("normal");
    //       mesh->updateBuffer("color");

//    meshBuffers["in_Vertex"]->write_dynamic(buffers["position"]);
//    meshBuffers["in_Normal"]->write_dynamic(buffers["normal"]);
    meshBuffers["in_Color"]->write_dynamic(*colorBuffer);

}

