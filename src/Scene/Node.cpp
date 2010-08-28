/*
 * Node.cpp
 *
 *  Created on: Jun 28, 2010
 *      Author: bmonkey
 */

#include "Node.h"

Node::Node(string name, vector<float> position, Mesh * mesh) {
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->size = 1;
}

Node::Node(string name, vector<float> position, Mesh * mesh, Material * material) {
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = 1;
}

Node::Node(string name, vector<float> position, float size, Mesh * mesh, Material * material){
    this->name = name;
    this->position = position;
    this->mesh = mesh;
    this->material = material;
    this->size = size;
}

void Node::setMesh(Mesh *mesh)
{
    this->mesh = mesh;
}

void Node::setPosition(vector<float> position)
{
    this->position = position;
}

string Node::getName() const
{
    return name;
}

vector<float> Node::getPosition()
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
