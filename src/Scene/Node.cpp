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
#include "Scene/SceneData.h"
#include "Renderer/RenderEngine.h"

Node::Node(string name, const QVector3D& position, float size,
    Mesh * mesh, Material * material)
:
        name(name), position(position), size(size), material(material),
         modelMatrix(QMatrix4x4()), castShadows(true),
        receiveShadows(false), mesh(mesh) {
    update();
    transparent = false;
    rotation = QVector3D();
}

void Node::setMesh(Mesh *mesh) {
    this->mesh = mesh;
}

void Node::setPosition(const QVector3D& position) {
    this->position = position;
    update();
}

void Node::setRotation(const QVector3D& rotation) {
    this->rotation = rotation;
    update();
}

string Node::getName() const {
    return name;
}

const QVector3D Node::getPosition() {
    return position;
}

const QVector3D Node::getRotation(){
  return rotation;
}

void Node::setName(string name) {
    this->name = name;
}

Material *Node::getMaterial() const {
    return material;
}

float Node::getSize() const {
    return size;
}

void Node::setMaterial(Material *material) {
    this->material = material;
}

void Node::draw() {
    material->activate();

    mesh->draw();
//    mesh->boundingBox->draw();
}

void Node::setSize(float size) {
    this->size = size;
    update();
}

Node::~Node() {
    // TODO(bmonkey): Auto-generated destructor stub
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

void Node::setCastShadows(bool castShadows) {
    this->castShadows = castShadows;
}

void Node::update() {
    modelMatrix.setToIdentity();
    modelMatrix.translate(position);
    modelMatrix.scale(size);
    modelMatrix.rotate(rotation.x(), QVector3D(1,0,0));
    modelMatrix.rotate(rotation.y(), QVector3D(0,1,0));
    modelMatrix.rotate(rotation.z(), QVector3D(0,0,1));
}

void Node::setView(
        ShaderProgram * shaderProgram, DirectionNode * viewPoint) {
    glError;
//    printf("Binding Node %s\n", name.c_str());
    shaderProgram->use();

    QMatrix4x4 tempMatrix = viewPoint->getView() * modelMatrix;
    shaderProgram->setUniform("MMatrix", modelMatrix);
    shaderProgram->setUniform("MVMatrix", tempMatrix);
    shaderProgram->setUniform("NormalMatrix", tempMatrix.normalMatrix());
    tempMatrix = viewPoint->getProjection() * tempMatrix;
    shaderProgram->setUniform("MVPMatrix", tempMatrix);

    glError;
}

void Node::setView(DirectionNode * viewPoint) {
	setView(material->getShaderProgram(), viewPoint);
}

QVector3D Node::getCenter() {
  return position + mesh->boundingBox->getCenter() * size;
}
