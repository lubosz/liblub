/*
 * Light.cpp
 *
 *  Created on: Oct 1, 2010
 *      Author: bmonkey
 */

#include "Light.h"
#include "SceneGraph.h"
#include "Materials.h"
#include "MeshFactory.h"
#include "Camera.h"

Light::Light(const QVector4D& position, const QVector3D & direction) {
	moveSensitivity = .1;
	this->position = position;
	this->direction = direction;
	Node * lightNode = new Node("Light",position.toVector3D(), MeshFactory::Instance().lamp(),new WhiteMat());
	lightNode->setCastShadows(false);
	SceneGraph::Instance().addNode(lightNode);
	viewMatrix = QMatrix4x4();
	projectionMatrix = QMatrix4x4();
	projectionMatrix.perspective(90,1920/1200,.1,10000);
	//projectionMatrix.perspective(70.0,1920/1200,0.1,1000.0);
	update();
}

Light::~Light() {
	// TODO Auto-generated destructor stub
}

QVector3D Light::getDirection() const
{
    return direction;
}

QVector4D Light::getPosition() const
{
    return position;
}

void Light::setColor(QVector4D & color)
{
    this->color = color;
}

void Light::setPosition(QVector4D & position)
{
    this->position = position;
}

void Light::bindShaderUpdate(ShaderProgram * shaderProgram){
	QVector4D lightPositionView = Camera::Instance().getView() * position;

    shaderProgram->setUniform(lightPositionView, "lightPositionView");

    QVector3D directionView = Camera::Instance().getView() * direction;
    shaderProgram->setUniform(directionView, "spotDirection");

    QVector3D spotDirectionView = Camera::Instance().getViewNoTranslation() * direction;
    spotDirectionView.normalize();
    shaderProgram->setUniform(spotDirectionView, "spotDirectionView");
}

void Light::bindShaderUpdateLight(ShaderProgram * shaderProgram){
	QVector4D lightPositionView = getView() * position;

    shaderProgram->setUniform(lightPositionView, "lightPositionView");

    QVector3D directionView = getView() * direction;
    shaderProgram->setUniform(directionView, "spotDirection");

    QVector3D spotDirectionView = getViewNoTranslation() * direction;
    spotDirectionView.normalize();
    shaderProgram->setUniform(spotDirectionView, "spotDirectionView");
}

void Light::bindShaderInit(ShaderProgram * shaderProgram){
	shaderProgram->setUniform(QVector4D(1.9, 1.9, 1.9,1.0), "lightColor");

	GLuint program = shaderProgram->getReference();
	//glUniform4f(glGetUniformLocation(program, "lightColor"), 0.8, 0.8, 0.8,1.0);

	//attenuation
	glUniform1f(glGetUniformLocation(program, "constantAttenuation"), 0);
	//glUniform1f(glGetUniformLocation(program, "linearAttenuation"), .8);
	glUniform1f(glGetUniformLocation(program, "quadraticAttenuation"), .005);

	//spot
	glUniform1f(glGetUniformLocation(program, "spotOuterAngle"), 0.9);
	glUniform1f(glGetUniformLocation(program, "spotInnerAngle"), 0.8);
	//cout << "Direction " << direction.x() << "\n";

	//shaderProgram->setUniform({1,1,1}, "spotDirection");

}

void Light::moveLeft(){
	position += QVector4D(-moveSensitivity,0,0,0);
    update();
}

void Light::moveRight(){
	position += QVector4D(moveSensitivity,0,0,0);
    update();
}

void Light::moveUp(){
	position += QVector4D(0,moveSensitivity,0,0);
    update();
}

void Light::moveDown(){
	position += QVector4D(0,-moveSensitivity,0,0);
    update();
}

void Light::moveForward(){
	position += QVector4D(0,0,moveSensitivity,0);
    update();
}

void Light::moveBack(){
	position += QVector4D(0,0,-moveSensitivity,0);
    update();
}

void Light::update(){
	SceneGraph::Instance().setPosition("Light", position.toVector3D());
	//cout << "Light: " << position.x() <<" "<< position.y()<<" " << position.z() << "\n";
	viewMatrix.setToIdentity();
	viewMatrix.lookAt(
			position.toVector3D(),
			direction,
			{0,1,0}
	);

}

QMatrix4x4 Light::getView() const{

	return viewMatrix;
}

QMatrix4x4 Light::getProjection() const{

	return projectionMatrix;
}

QMatrix4x4 Light::getViewNoTranslation() const{
	QMatrix4x4 viewMatrixNoTranslation;
	viewMatrixNoTranslation.lookAt(QVector3D(0,0,0),direction,up);
    return viewMatrixNoTranslation;
}
