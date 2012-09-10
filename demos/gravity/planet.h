#ifndef SIMPLANET_H
#define SIMPLANET_H

#include "Scene/Node.h"

const float massstab = 0.0001;
const double G = 6.67300e-11;

class SimPlanet {
public:
    string texturePath;
    QVector3D position;
    double radius;
    Node * node;

    double mass;

    QVector3D velocity;

    SimPlanet(string texturePath, const QVector3D &position, float radius);

    void init();

    void move();
    void accelerate(const QVector3D &acceleration);

    void influenceByPlanet(SimPlanet * planet);
};

#endif // SIMPLANET_H
