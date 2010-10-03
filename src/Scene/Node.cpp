/*
 * Node.cpp
 *
 *  Created on: Jun 28, 2010
 *      Author: bmonkey
 */

#include "Node.h"
#include "MeshFactory.h"

Node::Node(string name, const QVector3D& position, Mesh * mesh) {
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->size = 1;
    modelMatrix = QMatrix4x4();
    update();
}

Node::Node(string name, const QVector3D& position, Mesh * mesh, Material * material) {
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = 1;
    modelMatrix = QMatrix4x4();
    update();
}

Node::Node(string name, const QVector3D& position, float size, Mesh * mesh, Material * material){
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = size;
    modelMatrix = QMatrix4x4();
    update();
}

Node::Node(string name, const QVector3D& position, string mesh, Material * material){
    this->name = name;
    this->position = position;
    this->mesh = MeshFactory::Instance().load(mesh);
    this->material = material;
    this->size = 1;
    modelMatrix = QMatrix4x4();
    update();
}

void Node::setMesh(Mesh *mesh)
{
    this->mesh = mesh;
}

void Node::setPosition(const QVector3D& position)
{
    this->position = position;
    update();
}

string Node::getName() const
{
    return name;
}

const QVector3D& Node::getPosition()
{
    return position;
}

void Node::setName(string name)
{
    this->name = name;
}

Material *Node::getMaterial() const
{
    return material;
}

float Node::getSize() const
{
    return size;
}

void Node::setMaterial(Material *material)
{
    this->material = material;
}

void Node::draw(){
	material->activate();
	mesh->draw();
}

void Node::setSize(float size){
	this->size = size;
    update();
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

void Node::update(){
	modelMatrix.setToIdentity();
	modelMatrix.translate(position);
	modelMatrix.scale(size);
}

void Node::bindShaders(ShaderProgram * shaderProgram, const QMatrix4x4 & viewMatrix, const QMatrix4x4 & projectionMatrix){
	glError("Node::bindShaders",113);
	shaderProgram->use();

	QMatrix4x4 tempMatrix =  viewMatrix * modelMatrix;
	shaderProgram->setUniform(tempMatrix, "MVMatrix");
	shaderProgram->setUniform(tempMatrix.normalMatrix(), "NormalMatrix");
	tempMatrix =  projectionMatrix * tempMatrix;

	shaderProgram->setUniform(tempMatrix,"MVPMatrix");


    glError("Node::bindShaders",124);
}

void Node::bindShaders(const QMatrix4x4 & viewMatrix, const QMatrix4x4 & projectionMatrix){
	bindShaders(material->getShaderProgram(), viewMatrix,projectionMatrix);
}
