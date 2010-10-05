#include "SceneGraph.h"
#include "Camera.h"
#include "RenderEngine.h"
#include "Materials.h"
#include "MeshFactory.h"
#include <boost/foreach.hpp>

SceneGraph::SceneGraph(){
	bias = QMatrix4x4();
	bias.translate(.5,.5,.5);
	bias.scale(.5,.5,.5);
}

void SceneGraph::animate(float frameCount){
	/*
    rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
    */
}

void SceneGraph::setPosition(string nodeName, const QVector3D& position){
    BOOST_FOREACH( Node *node, sceneNodes )
    {
        if (node->getName() == nodeName) {
        	node->setPosition(position);
        }
    }
}

void SceneGraph::printMatrix(const QMatrix4x4 & matrix, string name){
	cout << name << "\n";
	for (int i = 0; i < 16; i++){
			cout << matrix.data()[i] << "\t";
		if(i%4 == 3) cout << "\n";
	}
}

void SceneGraph::setShadowCoords(Node * node, DirectionNode * viewPoint){

	QMatrix4x4 camViewToShadowMapMatrix =
			bias
			* SceneGraph::Instance().light->getProjection()
			* SceneGraph::Instance().light->getView()
			* Camera::Instance().getView().inverted();

	node->bindShaders(
			node->getMaterial()->getShaderProgram(),
			viewPoint
	);

	node->getMaterial()->getShaderProgram()->setUniform(camViewToShadowMapMatrix, "camViewToShadowMapMatrix");
}

void SceneGraph::drawNodes(DirectionNode * viewPoint){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	//if (node->getReceiveShadows()){
    	if(true){
    		setShadowCoords(node,viewPoint);
    	}
        node->bindShaders(viewPoint);
    	light->bindShaderUpdate(node->getMaterial()->getShaderProgram());
    	node->draw();
    }
    glError("SceneGraph",139);

}

void SceneGraph::drawNodes(Material * material){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	//if (node->getReceiveShadows()){
    	if(true){
    		setShadowCoords(node,& Camera::Instance());
    	}
        node->bindShaders(& Camera::Instance());
        light->bindShaderUpdate(node->getMaterial()->getShaderProgram());
        node->draw();
    }
    glError("SceneGraph",139);

}

void SceneGraph::drawNodesLight(Material * material){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	if(node->getCastShadows()){
			node->bindShaders(material->getShaderProgram(),light);
			light->bindShaderUpdate(material->getShaderProgram());
			node->mesh->draw();
    	}
    }
    glError("SceneGraph",139);

}

void SceneGraph::addNode(Node * node){
	sceneNodes.push_back(node);
}

void SceneGraph::meshCube(string file, float cubeSize, float step, Material * material){
	Mesh * mesh = MeshFactory::Instance().load(file);

	for (float x = -cubeSize/2.0; x<cubeSize/2.0; x+=step ){
		for (float y = -cubeSize/2.0; y<cubeSize/2.0; y+=step ){
			for (float z = -cubeSize/2.0; z<cubeSize/2.0; z+=step ){
				addNode(new Node("",{x,y,z}, mesh,material));
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
				addNode(new Node("",{z,x,y}, mesh, materials.at(position%materials.size())));
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
			addNode(new Node("",{x,y,-5+x}, mesh, materials.at(position%materials.size())));
			position++;
		}
	}
}
