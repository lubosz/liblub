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

vector<float> Node::getPosition() const
{
    return position;
}

void Node::setName(string name)
{
    this->name = name;
}

void Node::draw(){
	mesh->draw();
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}
