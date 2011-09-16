/*
 * Copyright © 2010 Lubosz Sarnecki
 * Node.cpp
 *
 *  Created on: Jun 28, 2010
 */
#include <string>
#include "Scene/Node.h"
#include "Mesh/MeshLoader.h"
#include "Scene/Camera.h"
#include "System/Logger.h"
#include "Scene/Scene.h"
#include "Scene/SceneGraph.h"
#include "Renderer/OpenGL.h"
#include <cassert>

Node::Node(string name, const QVector3D& position, float size, Mesh * mesh,
        ShaderProgram * material) :
    castShadows(true), receiveShadows(false), m_size(size), name(name),
            eulerRotationCache(QVector3D()), modelMatrix(QMatrix4x4()),
            shader(material), transparent(false), position(position),
            rotationMatrix(QMatrix4x4()), mesh(mesh) {
    update();
}

Node::~Node() {
    // TODO(bmonkey): Auto-generated destructor stub
}
void Node::setMesh(Mesh *mesh) {
    this->mesh = mesh;
}

void Node::setPosition(const QVector3D& position) {
    this->position = position;
    update();
}

void Node::setRotation(const QVector3D& rotation) {
    eulerRotationCache = rotation;
    setRotationX(rotation.x());
    setRotationY(rotation.y());
    setRotationZ(rotation.z());
    update();
}

string Node::getName() const {
    return name;
}
 QVector3D Node::getPosition() const {
    return position;
}

void Node::setName(string name) {
    this->name = name;
}

ShaderProgram *Node::getShader() const {
    return shader;
}

float Node::size() const {
    return m_size;
}

void Node::setMaterial(ShaderProgram *material) {
    this->shader = material;
}

void Node::draw() {
    assert(shader);
    draw(shader);
}

void Node::draw(ShaderProgram * material) {
    material->activateAndBindTextures();
    mesh->draw();
}

void Node::setSize(float size) {
    m_size = size;
    update();
}

bool Node::getReceiveShadows() const {
    return receiveShadows;
}

void Node::setReceiveShadows(bool receiveShadows) {
    this->receiveShadows = receiveShadows;
}

bool Node::getCastShadows() const {
    return castShadows;
}

void Node::setShadowCoords(DirectionNode * viewPoint) {
    setShadowCoords(shader, viewPoint);
}

void Node::setShadowCoords(ShaderProgram * shaderProgram, DirectionNode * viewPoint){
  unsigned countLights = 0;
  foreach(Light * light, Scene::Instance().lights) {
    string name = "camViewToShadowMapMatrixshadowDepthSource" + QString::number(countLights).toStdString();

    QMatrix4x4 camViewToShadowMapMatrix = SceneGraph::Instance().bias
            * light->getProjection()
            * light->getView()
            * viewPoint->getView().inverted();

    shaderProgram->use();
    shaderProgram->setUniform(name, camViewToShadowMapMatrix);
    countLights++;
  }
}

void Node::setCastShadows(bool castShadows) {
    this->castShadows = castShadows;
}

void Node::update() {
    modelMatrix.setToIdentity();
    modelMatrix.translate(position);
    modelMatrix.scale(size());
    modelMatrix = modelMatrix * rotationMatrix;
}

void Node::setView(
        ShaderProgram * shaderProgram, DirectionNode * viewPoint) {
    shaderProgram->use();

    QMatrix4x4 tempMatrix = viewPoint->getView() * modelMatrix;
    shaderProgram->setUniform("MMatrix", modelMatrix);
    shaderProgram->setUniform("MVMatrix", tempMatrix);
    shaderProgram->setUniform("NormalMatrix", tempMatrix.normalMatrix());
    tempMatrix = viewPoint->getProjection() * tempMatrix;
    shaderProgram->setUniform("MVPMatrix", tempMatrix);
    shaderProgram->setUniform("camPositionWorld",Scene::Instance().getCurrentCamera()->getPosition());
    glError;
}

void Node::setView(DirectionNode * viewPoint) {
	setView(shader, viewPoint);
}

QVector3D Node::getCenter() {
    return position + mesh->boundingBox->getCenter() * size();
}

void Node::setRotationX(float rotation) {
    eulerRotationCache.setX(rotation);
    updateRotationFromEuler();
}
void Node::setRotationY(float rotation) {
    eulerRotationCache.setY(rotation);
    updateRotationFromEuler();
}
void Node::setRotationZ(float rotation) {
    eulerRotationCache.setZ(rotation);
    updateRotationFromEuler();
}

void Node::updateRotationFromEuler() {
    rotationMatrix.setToIdentity();
    rotationMatrix.rotate(eulerRotationCache.x(), QVector3D(1, 0, 0));
    rotationMatrix.rotate(eulerRotationCache.y(), QVector3D(0, 1, 0));
    rotationMatrix.rotate(eulerRotationCache.z(), QVector3D(0, 0, 1));
    update();
}

void Node::setPositionX(float position) {
    this->position.setX(position);
    update();
}
void Node::setPositionY(float position) {
    this->position.setY(position);
    update();
}
void Node::setPositionZ(float position) {
    this->position.setZ(position);
    update();
}

float Node::positionX() const {
    return position.x();
}
float Node::positionY() const {
    return position.y();
}
float Node::positionZ() const {
    return position.z();
}

float Node::rotationX() const {
    return eulerRotationCache.x();
}
float Node::rotationY() const {
    return eulerRotationCache.y();
}
float Node::rotationZ() const {
    return eulerRotationCache.z();
}

QVector3D Node::direction() {
    // 3. row of the rotation matrix  is the unit vector
    // describing the direction in which you are facing.
    QVector3D direction = rotationMatrix.row(2).toVector3D();
    return direction;
}

void Node::setDirection(const QVector3D & direction) {
    rotationMatrix.setToIdentity();
    rotationMatrix.lookAt(QVector3D(), -direction, up);
}
