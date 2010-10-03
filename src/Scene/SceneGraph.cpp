#include "SceneGraph.h"
#include "Camera.h"
#include "RenderEngine.h"
#include "Materials.h"
#include "MeshFactory.h"
#include <boost/foreach.hpp>

SceneGraph::SceneGraph(){

	modelMatrix = QMatrix4x4();
}

void SceneGraph::animate(float frameCount){
	/*
    rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
    */
}

void SceneGraph::bindShaders(ShaderProgram * shaderProgram, const QMatrix4x4 & viewMatrix, const QMatrix4x4 & projectionMatrix){
	glError("SceneGraph::bindShaders",22);
	shaderProgram->use();

	modelMatrix =  viewMatrix * modelMatrix;
	shaderProgram->setUniform(modelMatrix, "MVMatrix");
	shaderProgram->setUniform(modelMatrix.normalMatrix(), "NormalMatrix");
	modelMatrix =  projectionMatrix * modelMatrix;

	shaderProgram->setUniform(modelMatrix,"MVPMatrix");


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

void SceneGraph::drawNodes(){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	modelMatrix.setToIdentity();

    	//
    	modelMatrix.translate(node->getPosition());
    	modelMatrix.scale(node->getSize());

        bindShaders(node->getMaterial()->getShaderProgram(),Camera::Instance().getView(),Camera::Instance().getProjection());
    	light->bindShaderUpdate(node->getMaterial()->getShaderProgram());
    	node->draw();
    }
    glError("SceneGraph",139);

}

void SceneGraph::drawNodes(ShaderProgram * shaderProgram){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	modelMatrix.setToIdentity();

    	//
    	modelMatrix.translate(node->getPosition());
    	modelMatrix.scale(node->getSize());

    	QMatrix4x4 bias = {
    			0.5, 0.0, 0.0, 0.0,
    			0.0, 0.5, 0.0, 0.0,
    			0.0, 0.0, 0.5, 0.0,
    			0.5, 0.5, 0.5, 1.0
    	};

/*
    	QMatrix4x4 bias = QMatrix4x4();
    	bias.translate(.5,.5,.5);
    	bias.scale(.5,.5,.5);
*/


/*
    	QMatrix4x4 bias = {
    				0.5, 0.0, 0.0, 0.5,
    				0.0, 0.5, 0.0, 0.5,
    				0.0, 0.0, 0.5, 0.5,
    				0.5, 0.5, 0.5, 1.0
    		};
*/


    	QMatrix4x4 camViewToShadowMapMatrix =
    			bias
    			* SceneGraph::Instance().light->getProjection()

    			//* modelMatrix
    			* SceneGraph::Instance().light->getView()
    			* Camera::Instance().getView().inverted()
    			;



    			//* modelMatrix





        bindShaders(shaderProgram,Camera::Instance().getView(),Camera::Instance().getProjection());
    	shaderProgram->setUniform(camViewToShadowMapMatrix, "camViewToShadowMapMatrix");


    	light->bindShaderUpdate(shaderProgram);
    	node->mesh->draw();
    }
    glError("SceneGraph",139);

}

void SceneGraph::drawNodesLight(ShaderProgram * shaderProgram){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	modelMatrix.setToIdentity();

    	//
    	modelMatrix.translate(node->getPosition());
    	modelMatrix.scale(node->getSize());

//        bindShaders(shaderProgram,Camera::Instance().getView(),Camera::Instance().getProjection());
        bindShaders(shaderProgram,light->getView(),light->getProjection());

    	light->bindShaderUpdate(shaderProgram);
    	node->mesh->draw();
    }
    glError("SceneGraph",139);

}

void SceneGraph::drawNodesLight(){
    BOOST_FOREACH( Node * node, sceneNodes )
    {
    	modelMatrix.setToIdentity();

    	//
    	modelMatrix.translate(node->getPosition());
    	modelMatrix.scale(node->getSize());

        bindShaders(node->getMaterial()->getShaderProgram(),light->getView(),light->getProjection());
    	light->bindShaderUpdateLight(node->getMaterial()->getShaderProgram());
    	node->draw();
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
