#include <QMatrix4x4>
#include <QVector3D>

#include "SceneGraph.h"
#include "Camera.h"
#include "RenderEngine.h"
#include "Materials.h"
#include "MeshFactory.h"
#include <boost/foreach.hpp>

SceneGraph::SceneGraph(){
	QMatrix4x4 * matrixTest = new QMatrix4x4();
	matrixTest->setToIdentity();
	lightPosition = {-2.0, 1.0, 2.0};
	addNode("Light",lightPosition, MeshFactory::Instance().lamp(),new WhiteMat());
	modelMatrix = new Matrix();
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

void SceneGraph::cameraTransform(){
    modelMatrix->rotate(Camera::Instance().yaw, X_AXIS);
    modelMatrix->rotate(Camera::Instance().pitch, Y_AXIS);
    modelMatrix->rotate(Camera::Instance().roll, Z_AXIS);

    //scale(modelmatrix, 0.5);
    modelMatrix->translate(Camera::Instance().x, Camera::Instance().y, Camera::Instance().z);
}


void SceneGraph::bindShaders(ShaderProgram * shaderProgram){
	shaderProgram->use();
	modelMatrix->bindNormalMatrix(shaderProgram);
	modelMatrix->bind(shaderProgram, "MVMatrix");
    /* multiply our modelmatrix and our projectionmatrix. Results are stored in modelmatrix */
	//shaderProgram->setProjectionMatrix(Camera::Instance().getProjectionmatrix());
    //multiply4x4(modelmatrix, Camera::Instance().getProjectionmatrix());
	modelMatrix->multiply(Camera::Instance().getProjectionmatrix());
    modelMatrix->bind(shaderProgram, "MVPMatrix");
    shaderProgram->setLightPosition(lightPosition.at(0),lightPosition.at(1),lightPosition.at(2));

    glError("SceneGraph::bindShaders",116);
}

void SceneGraph::setPosition(string nodeName, const vector<float> & position){
    BOOST_FOREACH( Node &node, sceneNodes )
    {
        if (node.getName() == nodeName) {
        	node.setPosition(position);
        }
    }
}

void SceneGraph::addNode(string name, const vector<float> & position, Mesh * mesh, Material * material){
	sceneNodes.push_back(Node(name, position, mesh, material));
    BOOST_FOREACH( Material* oldMaterial, materials )
    {
        if (oldMaterial == material) return;
    }
	materials.push_back(material);
}

void SceneGraph::drawNodes(){
	//gluLookAt(1, 0, 1, 1, 0, 0, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
    BOOST_FOREACH( Node node, sceneNodes )
    {
    	modelMatrix->identity();

    	modelMatrix->scale(node.getSize());
    	modelMatrix->translate(node.getPosition());
    	cameraTransform();

        bindShaders(node.getMaterial()->getShaderProgram());
        Camera::Instance().getProjectionmatrix()->transpose4x4();
        Camera::Instance().getProjectionmatrix()->bind(node.getMaterial()->getShaderProgram(),"invProjView");

    	node.draw();
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

void SceneGraph::setLightPosition(vector<float> lightPosition){
	this->lightPosition = lightPosition;
}

void SceneGraph::addNode(string name, const vector<float> & position, Mesh * mesh){
	sceneNodes.push_back(Node(name, position, mesh));
}

void SceneGraph::addNode(Node * node){
	sceneNodes.push_back(*node);
}

void SceneGraph::addNode(string name, string file, const vector<float> & position, Material * material){
	addNode(name,position, MeshFactory::Instance().load(file),material);
}

void SceneGraph::addNode(string file, const vector<float> & position, Material * material){
	addNode(file, file, position, material);
}

void SceneGraph::addNode(Mesh * mesh, const vector<float> & position, Material * material){
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
