#include "MandelBulb.h"

namespace fractal {

QVector3D MandelBulb::triplexPow(QVector3D vec, unsigned n) {
    double r = sqrt(vec.x()*vec.x() + vec.y() * vec.y() + vec.z() * vec.z());
    double theta = 0;
    if (vec.x() != 0)
        theta = n * atan(vec.y()/vec.x());
    double phi = 0;
    if (r != 0)
        phi = n * asin(vec.z()/r);
    return pow(r,n) * QVector3D(cos(theta)*cos(phi),
                                sin(theta)*sin(phi),
                                -sin(phi));
}

double MandelBulb::norm(QVector3D in) {
    return QVector3D::dotProduct(in, in);
}

double MandelBulb::abs(QVector3D in) {
    return sqrt(QVector3D::dotProduct(in, in));
}

QVector3D MandelBulb::triplexMult(QVector3D vec1, QVector3D vec2) {
    double r1 = sqrt(vec1.x()*vec1.x() + vec1.y()*vec1.y());
    double r2 = sqrt(vec2.x()*vec2.x() + vec2.y()*vec2.y());
    double a = 1.0d - vec1.z() * vec2.z()/(r1 * r2);

    QVector3D vector = QVector3D(
                a * (vec1.x()* vec2.x() - vec1.y()* vec2.y()),
                a * (vec2.x()* vec1.y() - vec1.x()* vec2.y()),
                -(r2 * vec1.z() + r1 * vec2.z()));
    return vector;
}

double MandelBulb::distanceEstimate(QVector3D c, unsigned n, unsigned maxIter) {
    QVector3D p = QVector3D(0, 0, 0);
    QVector3D dp = QVector3D(1, 0, 0);
    const QVector3D dir = QVector3D(1, 0, 0);
    double r;
    double dr = 1;
    unsigned i = 0;
    bool loop = true;

    while(loop) {
        QVector3D foo = triplexPow(p, n);
        p =  foo + c;
        r = abs(p);
        i++;
        loop = (i < maxIter && r < 2);
        if(loop) {
            dp = n * triplexMult(dp, triplexPow(p, n - 1)) + dir;
            dr = abs(dp);
        }
    }

    return 2 * r * log(r) / dr / 4.0d;

}

int MandelBulb::mandelbulb(QVector3D c, int max) {
    QVector3D p = QVector3D(0, 0, 0);
    int i = 0;
    while(i < max && norm(p) < 4) {
        p = triplexPow(p, 8) + c;
        i++;
    }
    return i;
}

void MandelBulb::setColor(GLubyte *voxels, int sizex4, int size2x4,
                     int x, int y, int z,
                     GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    voxels[(x*4) + (y * sizex4) + (z * size2x4)] = r;
    voxels[(x*4)+1 + (y * sizex4) + (z * size2x4)] = g;
    voxels[(x*4)+2 + (y * sizex4) + (z * size2x4)] = b;
    voxels[(x*4)+3 + (y * sizex4) + (z * size2x4)] = a;
}

void MandelBulb::setColor(vector<GLubyte> *voxels, GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    voxels->push_back(r);
    voxels->push_back(g);
    voxels->push_back(b);
    voxels->push_back(a);
}

void MandelBulb::generateVoxels(unsigned fromVoxel, unsigned toVoxel, vector<GLubyte> *voxels, int id, int size) {

    unsigned maxIter = 20;
    unsigned maxIter2 = 20;

    double from = -1.1;
    double to = 1.1;
    double range = fabs(from) + fabs(to);
    //    double step = range/size;

    double scale = range/double(size);

    unsigned voxelRange = toVoxel - fromVoxel;
    double z2;

    double distance = to;
    QColor color(0,0,0);

    //    double maxDistance = -100;
    //    double minDistance = 100;

    //    double distMin = -1.0d;
    //    double distMax = 2.0d;
    //    double distRange = fabs(distMin) + fabs(distMax);

    //        QMap< int, unsigned> distances;


    for(unsigned x = fromVoxel; x < toVoxel; x++) {
        LogInfo << id << 100*(x-fromVoxel)/voxelRange << "%";
        double xtrans = scale * x + from;

        for(int y = 0; y < size; y++) {
            double ytrans = scale * y + from;

            for(int z = 0; z < size; z++) {

                z2 = to;
                distance = to;
                double ztrans = scale * z + from;
                /*
                while(z2 >= -0.1 && Mandelbulb(QVector3D(xtrans, ytrans, ztrans), maxIter) < maxIter) {
                    z2 -= step;
                }
*/
                unsigned iter2 = 0;
                while(distance > 0.001 && z2 >= -0.1 && iter2 <= maxIter2) {
                    distance = distanceEstimate(QVector3D(xtrans, ytrans, ztrans), 8, maxIter);
                    z2 -= distance;
                    iter2++;
                }

                //                if (distance < minDistance)
                //                    minDistance = distance;

                //                if (distance > maxDistance)
                //                    maxDistance = distance;

                //                LogInfo << distance;
                /*
                double hue = 0.5;

                if (distance > 0) {
                    for (int scaleFactor = 3; scaleFactor > -3; scaleFactor--) {
                        double zoom = pow(10.0,scaleFactor);
                        //                        LogDebug << "ZoomTest" << zoom << scaleFactor;
                        if (distance <= 1.0d / zoom) {
                            hue = distance * zoom;
                            break;
                        }
                    }
                } else  {
                    hue = 1.0;
                }

*/
                //                int distanceInt = z2 * 10;
                //                if (distances.contains(distanceInt))
                //                    distances[distanceInt] = distances[distanceInt] + 1;
                //                else {
                //                    distances.insert(distanceInt, 0);
                //                }

                /*

                if (0 < z2 && z2 < 1.1) {
//                    color.setHsvF(hue, 1.0, 1.0, 0.1);
//                    color.setRgbF(1.0,1.0,1.0,0.1);
//                    color.setRgbF(1.0,0.0,0.0,0.1);
                    color.setRgb(0,0,0,0);
                } else if (z2 < 0){
                    color.setRgbF(0.0,1.0,0.0,0.1);
//                    color.setRgb(0,0,0,0);
                } else {
//                    color.setHsvF(hue, 0.5, 0.5, 0.1);
                    color.setRgb(0,0,0,0);
//                    color.setRgbF(0.0,0.0,1.0,0.1);
                }
*/

                //if (z2 > 1.1) {
                if (distance < 0) {
                    //                    color.setRgbF(0.0,1.0,0.0,0.1);
                    double hue = 0;
                    for (int scaleFactor = 3; scaleFactor > -2; scaleFactor--) {
                        double zoom = pow(10.0,scaleFactor);
                        //                        LogDebug << "ZoomTest" << zoom << scaleFactor;
                        if (distance <= 1.0d / zoom) {
                            hue = distance * zoom;
                            break;
                        }
                    }

                    if (hue < 0.0 || hue > 1.0)
                        hue = 0;

//                    if (hue > 0.5)
//                        hue -= 0.5;
//                    else
//                        hue += 0.5;

                    color.setHsvF(hue, 1.0, 1.0, 0.7);
//                    color.setRgbF(0,1,1,1);
                } else {
                    color.setRgb(0,0,0,0);
                }

                setColor(voxels, color.red(), color.green(), color.blue(), color.alpha());
            }
        }
    }

    LogInfo << "Thread" << id << "done.";
    //        LogInfo << "z2";
    //        foreach (int dist, distances.keys()) {
    //            LogInfo << "Distance" << dist << distances[dist];
    //        }
    //    LogInfo << "Distances Min" << minDistance << "Max" << maxDistance;

}
}
