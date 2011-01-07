/*
 * Copyright © 2010 Lubosz Sarnecki
 * Camera.cpp
 *
 *  Created on: Apr 1, 2010
 */

#include "Scene/Camera.h"
#include "Scene/SceneGraph.h"
#include "Mesh/MeshFactory.h"

Camera::Camera() {
  direction = QVector3D(0, 0, -1);
  position = QVector3D();
  defaultDirection = direction;
}

Camera::~Camera() {
  // TODO(bmonkey): Auto-generated destructor stub.
}

void Camera::forward() {
  QVector3D front = direction;
  front.normalize();
  position += speed*front;
  updateView();
}

void Camera::backward() {
  QVector3D front = direction;
  front.normalize();
  position -= speed*front;
  updateView();
}

void Camera::left() {
  QVector3D side = QVector3D::crossProduct(direction, up);
  side.normalize();
  position -= speed * side;
  updateView();
}

void Camera::right() {
  QVector3D side = QVector3D::crossProduct(direction, up);
  side.normalize();
  position += speed * side;
  updateView();
}

void Camera::setMouseLook(int mouseXrel, int mouseYrel) {
  pitch -= mouseSensitivity * mouseXrel;
  yaw -= mouseSensitivity * mouseYrel;
  if (yaw > 89) yaw = 89;
  if (yaw < -89) yaw = -89;

  updateRotation();
}

void Camera::setMouseZoom(int wheelX, int wheelY) {
  Logger::Instance().message << "\t" << wheelX << "\t" << wheelY;
  Logger::Instance().log("DEBUG", "Camera Wheel");
}

void Camera::updateRotation() {
  rotation.setToIdentity();
  rotation.rotate(pitch, 0, 1, 0);
  rotation.rotate(yaw, 1, 0, 0);
  direction = rotation * defaultDirection;
  direction.normalize();
  updateView();
}
