/*
 * Copyright © 2010 Lubosz Sarnecki
 * Camera.cpp
 *
 *  Created on: Apr 1, 2010
 */

#include "Scene/Camera.h"
#include "Scene/SceneGraph.h"
#include "Mesh/MeshLoader.h"
#include "System/Logger.h"

Camera::Camera() {
  update();
}

Camera::~Camera() {
  // TODO(bmonkey): Auto-generated destructor stub.
}

void Camera::setMouseLook(int mouseXrel, int mouseYrel, qreal mouseSensitivity) {
  rotation.setX(rotation.x() - mouseSensitivity * mouseYrel);
  rotation.setY(rotation.y() + mouseSensitivity * mouseXrel);
  if (rotation.x() > 89) rotation.setX(89);
  if (rotation.x() < -89) rotation.setX(-89);
  update();
}

void Camera::setMouseZoom(int wheelX, int wheelY) {
  LogDebug << "\t" << wheelX << "\t" << wheelY;
}

void Camera::update() {
  updateRotation();
  updateView();
}

void Camera::setUniforms(ShaderProgram * program, const QVector3D fromPosition){
  program->use();
  program->setUniform("cameraPosition", position-fromPosition);
  QVector3D cameraDistance = fromPosition - position;
  program->setUniform("cameraHeight", (float)cameraDistance.length());
  program->setUniform("cameraHeight2", float(cameraDistance.length()*cameraDistance.length()));
}
