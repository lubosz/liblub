/*
 * AABB.cpp
 *
 *  Created on: Mar 5, 2011
 *      Author: bmonkey
 */

#include "Mesh/AABB.h"
#include "Mesh/Mesh.h"

AABB::AABB(QVector3D min, QVector3D max) {
  this->min = min;
  this->max = max;
  center = (min+max) / 2.0;
  createMesh();
}

AABB::~AABB() {
  // TODO Auto-generated destructor stub
}

const QVector3D AABB::getCenter() {
  return center;
}

void AABB::draw() {
    mesh->draw();
}

void AABB::createMesh() {
  vector<GLfloat> vertices = {
          1.0, 1.0, 1.0,
          -1.0, -1.0, 1.0,
          -1.0, 1.0, -1.0,
          1.0, -1.0, -1.0
  };

  vector<GLuint> indicies = { 0, 1, 2, 3, 0, 1 };

  mesh = new Mesh();
  mesh->init();
  mesh->addBuffer(vertices, 3, "in_Vertex");
//  mesh->addBuffer(vertexColors, 3, "in_Color");
  mesh->addElementBuffer(indicies);
  mesh->setDrawType(GL_LINES);
}
