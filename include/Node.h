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
public:
	Mesh * mesh;
    bool getCastShadows() const;
    void setCastShadows(bool castShadows);
    bool getReceiveShadows() const;
    void setReceiveShadows(bool receiveShadows);
private:
	string name;
	//vector<float> position;
	QVector3D position;
	float size;
	Node * parent;
	QMatrix4x4 modelMatrix;
	Material * material;
	bool castShadows, receiveShadows;


public:
	Node(string name, const QVector3D& position, Mesh * mesh);
	Node(string name, const QVector3D& position, Mesh * mesh, Material * material);
	Node(string name, const QVector3D& position, string mesh, Material * material);
	Node(string name, const QVector3D& position, float size, Mesh * mesh, Material * material);
	virtual ~Node();

	void setSize(float size);
    void setMesh(Mesh *mesh);
    void setPosition(const QVector3D& position);
	void draw();
	void bindShaders(ShaderProgram * shaderProgram, const QMatrix4x4 & viewMatrix, const QMatrix4x4 & projectionMatrix);
	void bindShaders(const QMatrix4x4 & viewMatrix, const QMatrix4x4 & projectionMatrix);
    string getName() const;
    const QVector3D& getPosition();
    void setName(string name);
    Material *getMaterial() const;
    void setMaterial(Material *material);
    float getSize() const;
    void update();
};

