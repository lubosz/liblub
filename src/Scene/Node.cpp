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
}

Node::Node(string name, const QVector3D& position, Mesh * mesh, Material * material) {
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = 1;
}

Node::Node(string name, const QVector3D& position, float size, Mesh * mesh, Material * material){
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = size;
}

Node::Node(string name, const QVector3D& position, string mesh, Material * material){
    this->name = name;
    this->position = position;
    this->mesh = MeshFactory::Instance().loadAssimp(mesh);
    this->material = material;
    this->size = 1;
}

void Node::setMesh(Mesh *mesh)
{
    this->mesh = mesh;
}

void Node::setPosition(const QVector3D& position)
{
    this->position = position;
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
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}
