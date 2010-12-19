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
    castShadows = true;
    receiveShadows = false;
    update();
}

Node::Node(string name, const QVector3D& position, Mesh * mesh, Material * material) {
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = 1;
    modelMatrix = QMatrix4x4();
    castShadows = true;
    receiveShadows = false;
    update();
}

Node::Node(string name, const QVector3D& position, float size, Mesh * mesh, Material * material){
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = size;
    modelMatrix = QMatrix4x4();
    castShadows = true;
    receiveShadows = false;
    update();
}

Node::Node(string name, const QVector3D& position, string mesh, Material * material){
    this->name = name;
    this->position = position;
    this->mesh = MeshFactory::load(mesh);
    this->material = material;
    this->size = 1;
    modelMatrix = QMatrix4x4();
    castShadows = true;
    receiveShadows = false;
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


bool Node::getReceiveShadows() const
{
    return receiveShadows;
}

void Node::setReceiveShadows(bool receiveShadows)
{
    this->receiveShadows = receiveShadows;
}

bool Node::getCastShadows() const
{
    return castShadows;
}

void Node::setCastShadows(bool castShadows)
{
    this->castShadows = castShadows;
}

void Node::update(){
	modelMatrix.setToIdentity();
	modelMatrix.translate(position);
	modelMatrix.scale(size);
}

void Node::bindShaders(ShaderProgram * shaderProgram, DirectionNode * viewPoint){
	glError("Node::bindShaders",113);
	shaderProgram->use();

	QMatrix4x4 tempMatrix =  viewPoint->getView() * modelMatrix;

	shaderProgram->setUniform(tempMatrix, "MVMatrix");
	shaderProgram->setUniform(tempMatrix.normalMatrix(), "NormalMatrix");
	tempMatrix =  viewPoint->getProjection() * tempMatrix;

	shaderProgram->setUniform(tempMatrix,"MVPMatrix");

    glError("Node::bindShaders",124);
}

void Node::bindShaders(DirectionNode * viewPoint){
	bindShaders(material->getShaderProgram(), viewPoint);
}
