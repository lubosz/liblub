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


Node::Node() : castShadows(true), receiveShadows(false),
    m_size(1.0f), name("A Node"), eulerRotationCache(QVector3D()),
    modelMatrix(QMatrix4x4()), shader(nullptr),
    position(QVector3D()), rotation(QQuaternion()), mesh(nullptr) {
    material = new Material("Dummy material");
    update();
}

Node::Node(string name, const QVector3D& position, float size, Mesh * mesh,
        ShaderProgram * shaderProgram) :
    castShadows(true), receiveShadows(false), m_size(size), name(name),
            eulerRotationCache(QVector3D()), modelMatrix(QMatrix4x4()),
            shader(shaderProgram), position(position),
            rotation(QQuaternion()), mesh(mesh) {
    material = new Material("Dummy material");
    update();
}

Node::Node(string name, const QVector3D& position, float size, Mesh * mesh,
        Material * material) :
    castShadows(true), receiveShadows(false), m_size(size), name(name),
            eulerRotationCache(QVector3D()), modelMatrix(QMatrix4x4()),
            material(material), position(position),
            rotation(QQuaternion()), mesh(mesh) {
    update();
}

Node::~Node() {
    if(shader != nullptr)
        delete shader;
    if(mesh != nullptr)
        delete mesh;
}

Material * Node::getMaterial() const {
    if (material == nullptr)
        LogError << "no material";
    return material;
}

void Node::setMesh(Mesh *mesh) {
    this->mesh = mesh;
}

bool Node::hasShader() const {
    return (shader != nullptr);
}

void Node::setPosition(const QVector3D& position) {
    this->position = position;
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

void Node::setShader(ShaderProgram* shader) {
    this->shader = shader;
}

ShaderProgram *Node::getShader() const {
    assert(shader != nullptr);
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

void Node::draw(ShaderProgram * shaderProgram) {
    shaderProgram->activateAndBindTextures();
    mesh->draw();
//    mesh->boundingBox->draw();
}

void Node::draw(ShaderProgram * shader, DirectionNode * camView) {
    setView(shader, camView);
    if (material) {
        material->uniforms(shader);
        material->activateAndBindTextures();
    }
    draw(shader);
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

    if (rotation != QQuaternion())
        modelMatrix.rotate(rotation);
    else
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
    return position + mesh->getCenter() * size();
}

void Node::setRotationX(float rotationX) {
    eulerRotationCache.setX(rotationX);
    updateRotationFromEuler();
}
void Node::setRotationY(float rotationY) {
    eulerRotationCache.setY(rotationY);
    updateRotationFromEuler();
}
void Node::setRotationZ(float rotationZ) {
    eulerRotationCache.setZ(rotationZ);
    updateRotationFromEuler();
}

void Node::setRotation(const QQuaternion& rotationQuat) {
    rotation = rotationQuat;
    update();
}

void Node::setRotation(const QVector3D& rotationVec) {
//    rotation = QQuaternion(1, rotationVec);
    eulerRotationCache = rotationVec;
    setRotationX(rotationVec.x());
    setRotationY(rotationVec.y());
    setRotationZ(rotationVec.z());
    update();
}

void Node::setRotation(const QMatrix4x4& rotationMatrix) {
    this->rotationMatrix = rotationMatrix;
    update();
}

void Node::updateRotationFromEuler() {
    rotationMatrix.setToIdentity();
    rotationMatrix.rotate(eulerRotationCache.x(), QVector3D(1, 0, 0));
    rotationMatrix.rotate(eulerRotationCache.y(), QVector3D(0, 1, 0));
    rotationMatrix.rotate(eulerRotationCache.z(), QVector3D(0, 0, 1));
    update();
}

void Node::setPositionX(float positionX) {
    position.setX(positionX);
    update();
}
void Node::setPositionY(float positionY) {
    position.setY(positionY);
    update();
}
void Node::setPositionZ(float positionZ) {
    position.setZ(positionZ);
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
//    QMatrix4x4 rotationMatrix;
//    rotationMatrix.setToIdentity();
//    rotationMatrix.rotate(rotation);
    QVector3D direction = rotationMatrix.row(2).toVector3D();
    return direction;
}

void Node::setDirection(const QVector3D & direction) {
//    rotation = QQuaternion(1, direction);
    rotationMatrix.setToIdentity();
    rotationMatrix.lookAt(QVector3D(), -direction, up);
}


void Node::setTransparent(bool transparent) {
    material->transparent = transparent;
}

bool Node::getTransparent() {
    return material->transparent;
}

Mesh * Node::getMesh() {
    if (mesh == nullptr) {
        LogError << "Node" << name << "has no mesh!";
//        mesh = new Mesh(QList<string>());
        return nullptr;
    } else {
        return mesh;
    }
}
