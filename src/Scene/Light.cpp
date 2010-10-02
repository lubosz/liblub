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

Light::Light(QVector4D position) {
	moveSensitivity = .1;
	this->position = position;
	SceneGraph::Instance().addNode(new Node("Light",position.toVector3D(), MeshFactory::Instance().lamp(),new WhiteMat()));
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

void Light::setDirection(QVector3D direction)
{
    this->direction = direction;

}

void Light::setPosition(QVector4D & position)
{
    this->position = position;
}

void Light::update(){
	SceneGraph::Instance().setPosition("Light", position.toVector3D());
    //cout << "Light: " << position.x() <<" "<< position.y()<<" " << position.z() << "\n";
}

void Light::bindShaderUpdate(ShaderProgram * shaderProgram){
	QVector4D lightPositionView = Camera::Instance().getView() * position;

    shaderProgram->setUniform(lightPositionView, "lightPositionView");
    QVector3D spotDirectionView = Camera::Instance().getViewNoTranslation() * direction;
    spotDirectionView.normalize();
    shaderProgram->setUniform(spotDirectionView, "spotDirectionView");
    //shaderProgram->setUniform(direction, "spotDirectionView");
}

void Light::bindShaderInit(ShaderProgram * shaderProgram){
	shaderProgram->setUniform(QVector4D(1.9, 1.9, 1.9,1.0), "lightColor");

	GLuint program = shaderProgram->getReference();
	//glUniform4f(glGetUniformLocation(program, "lightColor"), 0.8, 0.8, 0.8,1.0);

	//attenuation
	glUniform1f(glGetUniformLocation(program, "constantAttenuation"), 0);
	//glUniform1f(glGetUniformLocation(program, "linearAttenuation"), .8);
	glUniform1f(glGetUniformLocation(program, "quadraticAttenuation"), .10);

	//spot
	glUniform1f(glGetUniformLocation(program, "spotOuterAngle"), 0.9);
	glUniform1f(glGetUniformLocation(program, "spotInnerAngle"), 0.6);
	glUniform3f(glGetUniformLocation(program, "spotDirection"), 5, 0, -5);

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
