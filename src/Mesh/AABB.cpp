/*
 * AABB.cpp
 *
 *  Created on: Mar 5, 2011
 *      Author: bmonkey
 */

#include "Mesh/AABB.h"
#include "Mesh/Mesh.h"

AABB::AABB() {
  mesh = nullptr;
  QVector3D min = QVector3D();
  QVector3D max = QVector3D();
  center = (min+max) / 2.0;
}

AABB::~AABB() {
  delete mesh;
}

void AABB::update(const QVector3D& position) {
    update(position.x(), position.y(), position.z());
}

void AABB::update(float x, float y, float z) {
    if (min.x() > x)
      min.setX(x);
    if (min.y() > y)
      min.setY(y);
    if (min.z() > z)
      min.setZ(z);
    if (max.x() < x)
      max.setX(x);
    if (max.y() < y)
      max.setY(y);
    if (max.z() < z)
      max.setZ(z);
}

const QVector3D AABB::getCenter() {
  return center;
}

void AABB::draw() {
    if (mesh == nullptr)
        createMesh();
    mesh->draw();
}

void AABB::createMesh() {
  mesh = new Mesh(QList<string>());
  mesh->buffers["position"] = {
          1.0, 1.0, 1.0,
          -1.0, -1.0, 1.0,
          -1.0, 1.0, -1.0,
          1.0, -1.0, -1.0
  };
  mesh->indices = { 0, 1, 2, 3, 0, 1 };
  mesh->init();
  mesh->setDrawType(GL_LINES);
}
