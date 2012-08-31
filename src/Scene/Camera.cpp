/*
 * Copyright © 2010 Lubosz Sarnecki
 * Camera.cpp
 *
 *  Created on: Apr 1, 2010
 */

#include "Scene/Camera.h"
#include "Scene/SceneGraph.h"
#include "Load/MeshLoader.h"
#include "System/Logger.h"

Camera::Camera() {
  setDirection(QVector3D(0,0,-1));
  update();
}

Camera::~Camera() {
  // TODO(bmonkey): Auto-generated destructor stub.
}

void Camera::setMouseLook(int mouseXrel, int mouseYrel, qreal mouseSensitivity) {
    rotationMatrix.rotate(mouseSensitivity * mouseXrel, QVector3D(0,1,0));
//    rotation = QQuaternion(mouseSensitivity * mouseXrel, QVector3D(0,1,0));

  QVector3D dir = direction();
  qreal upAngle = QVector3D::dotProduct(up, dir);

  // Don't damage rotation matrix when looking straigt up and down
  if((upAngle > -0.99 || mouseYrel < 0)
     && (upAngle < 0.99 || mouseYrel > 0)) {
//      rotation = QQuaternion(mouseSensitivity * mouseYrel, QVector3D::crossProduct(dir, up));
    rotationMatrix.rotate(mouseSensitivity * mouseYrel, QVector3D::crossProduct(dir, up));
  }
  update();
}

void Camera::setMouseZoom(int wheelX, int wheelY) {
  LogDebug << "\t" << wheelX << "\t" << wheelY;
}

void Camera::update() {
  updateView();
  updatePerspective();
}

void Camera::setUniforms(ShaderProgram * program, const QVector3D fromPosition){
  program->use();
  program->setUniform("cameraPosition", position-fromPosition);
  QVector3D cameraDistance = fromPosition - position;
  program->setUniform("cameraHeight", cameraDistance.length());
  program->setUniform("cameraHeight2", cameraDistance.length()*cameraDistance.length());
}
