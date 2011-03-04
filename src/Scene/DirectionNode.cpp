/*
 * Copyright © 2010 Lubosz Sarnecki
 * DirectionNode.cpp
 *
 *  Created on: Oct 5, 2010
 */

#include "Scene/DirectionNode.h"

DirectionNode::DirectionNode(){
  projectionMatrix = QMatrix4x4();
  rotation = QMatrix4x4();
  viewMatrix = QMatrix4x4();
  yaw = 0, pitch = 0, roll = 0;

  // TODO(bmonkey): Hardcoded values => xml
  speed = .1;
  mouseSensitivity = .1;

  aspect = 1920.0 / 1200.0;

  fov = 70.0;
  near = 0.1;
  far = 1000.0;
}

DirectionNode::~DirectionNode(){

}

QVector3D DirectionNode::getDirection() const {
    return direction;
}

QVector3D DirectionNode::getPosition() const {
    return position;
}

QMatrix4x4 DirectionNode::getProjection() const {
    return projectionMatrix;
}

QMatrix4x4 DirectionNode::getView() const {
    return viewMatrix;
}

// TODO(bmonkey): This shouldn't be needed.
QMatrix4x4 DirectionNode::getViewNoTranslation() const {
    QMatrix4x4 viewMatrixNoTranslation;
    viewMatrixNoTranslation.lookAt(QVector3D(0, 0, 0), direction, up);
    return viewMatrixNoTranslation;
}

void DirectionNode::setPosition(const QVector3D & position) {
    this->position = position;
}

void DirectionNode::setDirection(const QVector3D & direction) {
    this->direction = direction;
}

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void DirectionNode::updatePerspective() {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fov, aspect, near, far);
}

void DirectionNode::updateView() {
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(position, direction + position, up);
}

void DirectionNode::setParams(qreal fov, qreal near, qreal far) {
    this->fov = fov;
    this->near = near;
    this->far = far;
}

void DirectionNode::setAspect(qreal aspect) {
    this->aspect = aspect;
    /* Create our projection matrix with a 45 degree field of view
     * a width to height ratio of 1 and view from .1 to 100 infront of us */
    updatePerspective();
}
