#include "planet.h"

#include "Material/Shaders.h"
#include "Material/Textures.h"
#include "Scene/SceneGraph.h"
#include "Mesh/Geometry.h"

Planet::Planet(string texturePath, const QVector3D &position, float radius) : texturePath(texturePath) {
    this->position = position;
    this->radius = radius;
}

void Planet::init() {
    QList<string> attributes = QList<string>() << "uv";
    ShaderProgram * shader = new VertFragProgram("Texture/texture", attributes);
    Texture * texture = new TextureFile(texturePath, "diffuse");
    texture->bind();
    texture->filterMinMag(GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
    shader->addTexture(texture);

    Mesh * mesh = Geometry::sphere(attributes, massstab * radius, 20, 20);
    node = new Node("planet", massstab * position, 1, mesh, shader);
    node->setRotation(QVector3D(-90,0,0));
    SceneGraph::Instance().addNode(node);
}

void Planet::move() {
    position += velocity;
    node->position += velocity * massstab;
    node->update();

    float rotation = node->rotationZ() + .1;
    node->setRotationZ(rotation);
    node->updateRotationFromEuler();
}

void Planet::influenceByPlanet(Planet * planet) {
    QVector3D distanceVector = (planet->position - position) * 1000;
    double r = distanceVector.length();
    QVector3D moveDir = distanceVector.normalized();

    if (r < (radius + planet->radius) * 1000) {
//        LogInfo << "collision";

        QVector3D reversed = QVector3D::dotProduct(moveDir, velocity) * moveDir;

        velocity = -reversed;
    }

    // F= G* mM / r^2
    // F = m^3 / kg s^2 * kg^2 / m^2
    // F = kg * m / s ^ 2
//    double F = G * (planet->mass * mass / (r * r));

    // F= m*a
    // a = F  / m
//    double acceleration = F / mass;

    // a = m / s ^ 2
    double acceleration = G * (planet->mass / (r * r));
    accelerate(acceleration * moveDir);
}

void Planet::accelerate(const QVector3D &acceleration) {
    velocity += acceleration;
//    LogDebug << "velocity" << velocity.x() << velocity.y() << velocity.z()
//             << "acceleration" << acceleration.x() << acceleration.y() << acceleration.z();
}
