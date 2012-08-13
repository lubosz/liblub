/*
 * Copyright © 2010 Lubosz Sarnecki
 * DirectionNode.cpp
 *
 *  Created on: Oct 5, 2010
 */

#include "Scene/DirectionNode.h"

DirectionNode::DirectionNode(){
  projectionMatrix = QMatrix4x4();
  viewMatrix = QMatrix4x4();

  // TODO(bmonkey): Hardcoded values => xml
  aspect = 1920.0 / 1200.0;

  fov = 70.0;
  nearClip = 0.01;
  farClip = 1000.0;
  updatePerspective();
}

DirectionNode::~DirectionNode(){

}

QMatrix4x4 DirectionNode::getProjection() const {
    return projectionMatrix;
}

QMatrix4x4 DirectionNode::getView() const {
    return viewMatrix;
}

// TODO(bmonkey): This shouldn't be needed.
QMatrix4x4 DirectionNode::getViewNoTranslation() {
    QMatrix4x4 viewMatrixNoTranslation;
    viewMatrixNoTranslation.lookAt(QVector3D(0, 0, 0), direction(), up);
    return viewMatrixNoTranslation;
}

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void DirectionNode::updatePerspective() {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fov, aspect, nearClip, farClip);
}

void DirectionNode::updateView() {
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(position, direction() + position, up);
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
    position += distance*direction();
    updateView();
}

void DirectionNode::backwardDirection(qreal distance) {
    position -= distance*direction();
    updateView();
}

void DirectionNode::leftDirection(qreal distance) {
    updateSideDirection();
    position -= distance * sideDirection;
    updateView();
}

void DirectionNode::rightDirection(qreal distance) {
    updateSideDirection();
    position += distance * sideDirection;
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

void DirectionNode::updateSideDirection() {
    sideDirection = QVector3D::crossProduct(direction(), up);
    sideDirection.normalize();
}
