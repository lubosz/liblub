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
  aspect = 1920.0 / 1200.0;

  fov = 70.0;
  nearClip = 0.1;
  farClip = 1000.0;
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
    projectionMatrix.perspective(fov, aspect, nearClip, farClip);
}

void DirectionNode::updateView() {
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(position, direction + position, up);
}

void DirectionNode::setParams(qreal fov, qreal near, qreal far) {
    this->fov = fov;
    this->nearClip = near;
    this->farClip = far;
}

void DirectionNode::setAspect(qreal aspect) {
    this->aspect = aspect;
    /* Create our projection matrix with a 45 degree field of view
     * a width to height ratio of 1 and view from .1 to 100 infront of us */
    updatePerspective();
}


void DirectionNode::forwardDirection(qreal distance) {
  QVector3D front = direction;
  front.normalize();
  position += distance*front;
  updateView();
}

void DirectionNode::backwardDirection(qreal distance) {
  QVector3D front = direction;
  front.normalize();
  position -= distance*front;
  updateView();
}

void DirectionNode::leftDirection(qreal distance) {
  QVector3D side = QVector3D::crossProduct(direction, up);
  side.normalize();
  position -= distance * side;
  updateView();
}

void DirectionNode::rightDirection(qreal distance) {
  QVector3D side = QVector3D::crossProduct(direction, up);
  side.normalize();
  position += distance * side;
  updateView();
}

void DirectionNode::leftWorld(qreal distance) {
    position += QVector3D(-distance, 0, 0);
    update();
}

void DirectionNode::rightWorld(qreal distance) {
    position += QVector3D(distance, 0, 0);
    update();
}

void DirectionNode::upWorld(qreal distance) {
    position += QVector3D(0, distance, 0);
    update();
}

void DirectionNode::downWorld(qreal distance) {
    position += QVector3D(0, -distance, 0);
    update();
}

void DirectionNode::forwardWorld(qreal distance) {
    position += QVector3D(0, 0, distance);
    update();
}

void DirectionNode::backWorld(qreal distance) {
    position += QVector3D(0, 0, -distance);
    update();
}
