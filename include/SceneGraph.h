#pragma once

#include <QMatrix4x4>
#include "common.h"
#include "ShaderProgram.h"
#include "Singleton.h"
#include "Node.h"
#include "Material.h"
#include "Light.h"

class SceneGraph: public Singleton<SceneGraph>
{
public:
	Light * light;
	QMatrix4x4 modelMatrix;

	void updateLight();
	void drawNodesLight();
	void drawNodes(ShaderProgram * shaderProgram);
	void drawNodes();

	void addNode(Node * node);
	void meshPlane(string file, float cubeSize, float step, vector<Material*> materials);

	void bindShaders(ShaderProgram * shaderProgram, const QMatrix4x4 & viewMatrix, const QMatrix4x4 & projectionMatrix);
	void setPosition(string nodeName, const QVector3D& position);

private:
	friend class Singleton<SceneGraph>;

	vector<Node*> sceneNodes;
	//vector<Material*> materials;

    SceneGraph();

	void animate(float frameCount);
	void transform(float frameCount);

	void meshCube(string file, float cubeSize, float step, Material * material);
	void meshCube(string file, float cubeSize, float step, vector<Material*> materials);

};
