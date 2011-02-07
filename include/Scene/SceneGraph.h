/*
 * Copyright © 2010 Lubosz Sarnecki
 */
#pragma once

#include "Node.h"
#include "Light.h"

class SceneGraph: public Singleton<SceneGraph> {
 public:
	Light * light;

	void updateLight();
	void drawCasters(Material * material);
	void drawNodes(DirectionNode * viewPoint);
	void printMatrix(const QMatrix4x4 & matrix, string name);

	void addNode(Node * node);
	void meshPlane(
	        Mesh * mesh, float cubeSize, float step, vector<Material*> materials
	);

	Node * getNode(const string & name);
	bool hasNode(const string & name);

	void bindShaders(
	        ShaderProgram * shaderProgram, const QMatrix4x4 & viewMatrix,
	        const QMatrix4x4 & projectionMatrix
	);
	void setPosition(string nodeName, const QVector3D& position);

 private:
	friend class Singleton<SceneGraph>;

	vector<Node*> sceneNodes;
	QMatrix4x4 bias;

    SceneGraph();

//	void animate(float frameCount);
	void transform(float frameCount);

	void meshCube(string file, float cubeSize, float step, Material * material);
	void meshCube(
	        string file, float cubeSize, float step, vector<Material*> materials
	);

	void setShadowCoords(Node * node, DirectionNode * viewPoint);
};
