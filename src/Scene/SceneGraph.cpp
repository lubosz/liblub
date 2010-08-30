#include "SceneGraph.h"
#include "Camera.h"
#include "RenderEngine.h"
#include "Materials.h"
#include "MeshFactory.h"
#include <math.h>
#include <boost/foreach.hpp>

SceneGraph::SceneGraph(){
	lightPosition = {-2.0, 1.0, 2.0};
	addNode("Light",lightPosition, MeshFactory::Instance().lamp(),new WhiteMat());
}

void SceneGraph::updateLight(){
	setPosition("Light", lightPosition);
}

/* Multiply 4x4 matrix m1 by 4x4 matrix m2 and store the result in m1 */
void SceneGraph::multiply4x4(GLfloat *m1, GLfloat *m2)
{
    GLfloat temp[16];

    int x,y;

    for (x=0; x < 4; x++)
    {
        for(y=0; y < 4; y++)
        {
            temp[y + (x*4)] = (m1[x*4] * m2[y]) +
                              (m1[(x*4)+1] * m2[y+4]) +
                              (m1[(x*4)+2] * m2[y+8]) +
                              (m1[(x*4)+3] * m2[y+12]);
        }
    }

    memcpy(m1, temp, sizeof(GLfloat) * 16);
}



/* Perform translation operations on a matrix */
void SceneGraph::translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

    newmatrix[12] = x;
    newmatrix[13] = y;
    newmatrix[14] = z;

    multiply4x4(matrix, newmatrix);
}

/* Perform scale operations on a matrix */
void SceneGraph::scale(GLfloat *matrix, GLfloat size)
{
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

/*
    newmatrix[0] = size;
    newmatrix[5] = size;
    newmatrix[10] = size;
*/
    newmatrix[15] = 1/size;
    //cout << "size:" << size << "\n";
    multiply4x4(matrix, newmatrix);
}

/* Rotate a matrix by an angle on a X, Y, or Z axis */
void SceneGraph::rotate(GLfloat *matrix, GLfloat angle, AXIS axis)
{
    const GLfloat d2r = 0.0174532925199; /* PI / 180 */
    const int cos1[3] = { 5, 0, 0 };
    const int cos2[3] = { 10, 10, 5 };
    const int sin1[3] = { 6, 2, 1 };
    const int sin2[3] = { 9, 8, 4 };
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

    newmatrix[cos1[axis]] = cos(d2r * angle);
    newmatrix[sin1[axis]] = -sin(d2r * angle);
    newmatrix[sin2[axis]] = -newmatrix[sin1[axis]];
    newmatrix[cos2[axis]] = newmatrix[cos1[axis]];

    multiply4x4(matrix, newmatrix);
}

void SceneGraph::transpose3x3(GLfloat *matrix)
{
   GLfloat temp;
   for(int i=0;i<3;i++)
       for(int j=i+1;j<3;j++)
      {
         temp=matrix[i + (j*3)];
         matrix[i + (j*3)]=matrix[j + (i*3)];
         matrix[j + (i*3)]=temp;
      }
}

void SceneGraph::animate(float frameCount){
    rotate(modelmatrix, (GLfloat) frameCount * -1.0, X_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 1.0, Y_AXIS);
    rotate(modelmatrix, (GLfloat) frameCount * 0.5, Z_AXIS);
}

void SceneGraph::initNode(){
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
}

void SceneGraph::cameraTransform(){
    rotate(modelmatrix, (GLfloat) Camera::Instance().yaw, X_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().pitch, Y_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().roll, Z_AXIS);

    //scale(modelmatrix, 0.5);
    translate(modelmatrix, Camera::Instance().x, Camera::Instance().y, Camera::Instance().z);
}

void SceneGraph::transform(float frameCount){
    memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
    //animate(frameCount);
    rotate(modelmatrix, (GLfloat) Camera::Instance().yaw, X_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().pitch, Y_AXIS);
    rotate(modelmatrix, (GLfloat) Camera::Instance().roll, Z_AXIS);
    translate(modelmatrix, Camera::Instance().x, Camera::Instance().y, Camera::Instance().z);
}

void SceneGraph::translate(float x, float y, float z){
    translate(modelmatrix, x, y, z);
}

void SceneGraph::scale(float size){
    scale(modelmatrix, size);
}

void SceneGraph::translate(vector<float> translation){
    translate(modelmatrix, translation.at(0), translation.at(1), translation.at(2));
}

void SceneGraph::bindShaders(ShaderProgram * shaderProgram){

	shaderProgram->use();
    shaderProgram->setNormalMatrix(modelmatrix);
	shaderProgram->setModelViewMatrix(modelmatrix);
    /* multiply our modelmatrix and our projectionmatrix. Results are stored in modelmatrix */
    multiply4x4(modelmatrix, Camera::Instance().getProjectionmatrix());
    shaderProgram->setModelViewProjectionMatrix(modelmatrix);
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
    BOOST_FOREACH( Node node, sceneNodes )
    {
    	initNode();
        scale(node.getSize());
        translate(node.getPosition());
    	cameraTransform();


        bindShaders(node.getMaterial()->getShaderProgram());
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
