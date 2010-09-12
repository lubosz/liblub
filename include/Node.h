/*
 * Node.h
 *
 *  Created on: Jun 28, 2010
 *      Author: bmonkey
 */

#pragma once

#include "QVector3D"
#include "Mesh.h"
#include "Material.h"


class Node {
private:
	string name;
	//vector<float> position;
	QVector3D position;
	float size;
	Node * parent;
	Mesh * mesh;
	Material * material;


public:
	Node(string name, const QVector3D& position, Mesh * mesh);
	Node(string name, const QVector3D& position, Mesh * mesh, Material * material);
	Node(string name, const QVector3D& position, float size, Mesh * mesh, Material * material);
	virtual ~Node();

	void setSize(float size);
    void setMesh(Mesh *mesh);
    void setPosition(const QVector3D& position);
	void draw();
    string getName() const;
    const QVector3D& getPosition();
    void setName(string name);
    Material *getMaterial() const;
    void setMaterial(Material *material);
    float getSize() const;
};

