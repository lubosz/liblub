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

Light::Light(const QVector3D& position, const QVector3D & direction) {

	this->position = position;
	this->direction = direction;
	Node * lightNode = new Node("Light",position, MeshFactory::lamp(),new Simple("Color/white"));
	lightNode->setCastShadows(false);
	SceneGraph::Instance().addNode(lightNode);

	fov = 90;
	aspect = 1920/1200;
	near = .1;
	far = 10000;

	defaultValues();
	updatePerspective();
	update();
}

Light::~Light() {
	// TODO Auto-generated destructor stub
}

void Light::setColor(QVector4D & color)
{
    this->color = color;
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
	position += QVector3D(-speed,0,0);
    update();
}

void Light::moveRight(){
	position += QVector3D(speed,0,0);
    update();
}

void Light::moveUp(){
	position += QVector3D(0,speed,0);
    update();
}

void Light::moveDown(){
	position += QVector3D(0,-speed,0);
    update();
}

void Light::moveForward(){
	position += QVector3D(0,0,speed);
    update();
}

void Light::moveBack(){
	position += QVector3D(0,0,-speed);
    update();
}

void Light::update(){
	SceneGraph::Instance().setPosition("Light", position);
	Logger::Instance().message << position.x() << " " << position.y() << " "<< position.z();
    Logger::Instance().log("DEBUG","Light Position");
	updateView();
}
