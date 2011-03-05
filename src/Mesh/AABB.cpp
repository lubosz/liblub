/*
 * AABB.cpp
 *
 *  Created on: Mar 5, 2011
 *      Author: bmonkey
 */

#include "Mesh/AABB.h"

AABB::AABB(QVector3D min, QVector3D max) {
  this->min = min;
  this->max = max;
  center = (min+max) / 2.0;

}

AABB::~AABB() {
  // TODO Auto-generated destructor stub
}

const QVector3D& AABB::getCenter() {
  return center;
}
