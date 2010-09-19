#include "SceneGraph.h"
#include "Camera.h"
#include "RenderEngine.h"
#include "Materials.h"
#include "MeshFactory.h"
#include <boost/foreach.hpp>

SceneGraph::SceneGraph(){
	lightPosition = {-2.0, 1.0, 2.0};
	addNode("Light",lightPosition, MeshFactory::Instance().lamp(),new WhiteMat());
	modelMatrix = QMatrix4x4();
}

void SceneGraph::updateLight(){
	setPosition("Light", lightPosition);
}

void SceneGraph::animate(float frameCount){
	/*
    rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
    */
}

void SceneGraph::bindShaders(ShaderProgram * shaderProgram){
	glError("SceneGraph::bindShaders",42);
	shaderProgram->use();

	modelMatrix = Camera::Instance().getView() * modelMatrix;
	shaderProgram->setUniform(modelMatrix, "MVMatrix");
	shaderProgram->setUniform(modelMatrix.normalMatrix(), "NormalMatrix");
	modelMatrix = Camera::Instance().getProjection() * modelMatrix;

	shaderProgram->setUniform(modelMatrix,"MVPMatrix");
    shaderProgram->setLightPosition(lightPosition);

    glError("SceneGraph::bindShaders",53);
}

void SceneGraph::setPosition(string nodeName, const QVector3D& position){
    BOOST_FOREACH( Node *node, sceneNodes )
    {
        if (node->getName() == nodeName) {
        	node->setPosition(position);
        }
    }
}

void SceneGraph::addNode(string name, const QVector3D& position, Mesh * mesh, Material * material){
	sceneNodes.push_back(new Node(name, position, mesh, material));
    BOOST_FOREACH( Material* oldMaterial, materials )
    {
        if (oldMaterial == material) return;
    }
	materials.push_back(material);
}

void SceneGraph::drawNodes(){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	modelMatrix.setToIdentity();

    	//
    	modelMatrix.translate(node->getPosition());
    	modelMatrix.scale(node->getSize());

        bindShaders(node->getMaterial()->getShaderProgram());

    	node->draw();
    }
    glError("SceneGraph",139);

}

void SceneGraph::initUniforms(){
	cout << "Initializing uniforms for " << materials.size() << " Materials\n\n\n";
    BOOST_FOREACH( Material* material, materials )
    {
        material->initUniforms();
    }
    glError("SceneGraph",148);

}

void SceneGraph::setLightPosition(const QVector3D& lightPosition){
	this->lightPosition = lightPosition;
}

void SceneGraph::addNode(string name, const QVector3D& position, Mesh * mesh){
	sceneNodes.push_back(new Node(name, position, mesh));
}

void SceneGraph::addNode(Node * node){
	sceneNodes.push_back(node);
}

void SceneGraph::addNode(string name, string file, const QVector3D& position, Material * material){
	addNode(name,position, MeshFactory::Instance().load(file),material);
}

void SceneGraph::addNode(string file, const QVector3D& position, Material * material){
	addNode(file, file, position, material);
}

void SceneGraph::addNode(Mesh * mesh, const QVector3D& position, Material * material){
	addNode("blubb",position, mesh, material);
}

void SceneGraph::meshCube(string file, float cubeSize, float step, Material * material){
	Mesh * mesh = MeshFactory::Instance().load(file);

	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(mesh,{x,y,z}, material);
			}
		}
	}
}

void SceneGraph::meshCube(string file, float cubeSize, float step, vector<Material*> materials){
	Mesh * mesh = MeshFactory::Instance().load(file);
	unsigned position = 0;
	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(mesh,{z,x,y}, materials.at(position%materials.size()));
				position++;
			}
		}
	}
}

void SceneGraph::meshPlane(string file, float cubeSize, float step, vector<Material*> materials){
	Mesh * mesh = MeshFactory::Instance().load(file);
	unsigned position = 0;
	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			addNode(mesh,{x,y,-5+x}, materials.at(position%materials.size()));
			position++;
		}
	}
}
