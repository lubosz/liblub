#pragma once

#include "common.h"
#include "ShaderProgram.h"
#include "Singleton.h"
#include "Node.h"
#include "Material.h"
#include "Matrix.h"


class SceneGraph: public Singleton<SceneGraph>
{
public:
	vector<float> lightPosition;
	void updateLight();
	void drawNodes();

	void addNode(Node * node);
	void addNode(Mesh * mesh, const vector<float> & position, Material * material);
	void addNode(string file, const vector<float> & position, Material * material);

	void meshPlane(string file, float cubeSize, float step, vector<Material*> materials);


	void bindShaders(ShaderProgram * shaderProgram);

	Matrix * modelMatrix;

private:
	friend class Singleton<SceneGraph>;

	vector<Node> sceneNodes;
	vector<Material*> materials;


    SceneGraph();

	void animate(float frameCount);
	void transform(float frameCount);

	void cameraTransform();


	void initUniforms();
	void setPosition(string nodeName, const vector<float> & position);

	void setLightPosition(vector<float> lightPosition);


	void addNode(string name, string file,  const vector<float> & position, Material * material);
	void addNode(string name, const vector<float> & position, Mesh * mesh);
	void addNode(string name, const vector<float> & position, Mesh * mesh, Material * material);

	void meshCube(string file, float cubeSize, float step, Material * material);
	void meshCube(string file, float cubeSize, float step, vector<Material*> materials);


};
