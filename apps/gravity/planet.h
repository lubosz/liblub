#ifndef PLANET_H
#define PLANET_H

#include "Scene/Node.h"

const float massstab = 0.0001;
const double G = 6.67300e-11;
//const float massstab = 0.0001;

class Planet {
public:
    string texturePath;
    QVector3D position;
    double radius;
    Node * node;

    double mass;

    QVector3D velocity;


    Planet(string texturePath, const QVector3D &position, float radius);

    void init();

    void move();
    void accelerate(const QVector3D &acceleration);

    void influenceByPlanet(Planet * planet);
};

#endif // PLANET_H
