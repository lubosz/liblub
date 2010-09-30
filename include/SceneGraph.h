#pragma once

#include <QMatrix4x4>
#include "common.h"
#include "ShaderProgram.h"
#include "Singleton.h"
#include "Node.h"
#include "Material.h"

class SceneGraph: public Singleton<SceneGraph>
{
public:
	QVector4D lightPosition;
	QMatrix4x4 modelMatrix;

	void updateLight();
	void drawNodes();

	void addNode(Node * node);
	void addNode(Mesh * mesh, const QVector3D& position, Material * material);
	void addNode(string file, const QVector3D& position, Material * material);

	void meshPlane(string file, float cubeSize, float step, vector<Material*> materials);

	void bindShaders(ShaderProgram * shaderProgram);
	void setPosition(string nodeName, const QVector3D& position);

private:
	friend class Singleton<SceneGraph>;

	vector<Node*> sceneNodes;
	vector<Material*> materials;

    SceneGraph();

	void animate(float frameCount);
	void transform(float frameCount);

	void initUniforms();



	void setLightPosition(const QVector3D& lightPosition);

	void addNode(string name, string file,  const QVector3D& position, Material * material);
	void addNode(string name, const QVector3D& position, Mesh * mesh);
	void addNode(string name, const QVector3D& position, Mesh * mesh, Material * material);

	void meshCube(string file, float cubeSize, float step, Material * material);
	void meshCube(string file, float cubeSize, float step, vector<Material*> materials);

};
