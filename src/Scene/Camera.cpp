/*
 * Camera.cpp
 *
 *  Created on: Apr 1, 2010
 *      Author: bmonkey
 */


#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>

#include "Camera.h"
#include "common.h"
#include "SceneGraph.h"
#include "MeshFactory.h"
#include "Materials.h"

Camera::Camera() {
	projectionMatrix = QMatrix4x4();
	viewMatrix = QMatrix4x4();
	eye = QVector3D();
	center = QVector3D(0,0,-1);
	defaultCenter = center;
    yaw, pitch, roll = 0;
    speed = .1;
    mouseSensitivity = .1;
    centerNode = new Node("LookAt",center, .1, MeshFactory::Instance().cube(),new WhiteMat());
    //SceneGraph::Instance().addNode(centerNode);
}

QMatrix4x4 Camera::getProjection() const
{
    return projectionMatrix;
}

QMatrix4x4 Camera::getView() const
{
    return viewMatrix;
}

QMatrix4x4 Camera::getViewNoTranslation() const
{
	QMatrix4x4 viewMatrixNoTranslation;
	viewMatrixNoTranslation.lookAt(QVector3D(0,0,0),center,up);
    return viewMatrixNoTranslation;
}


Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::setAspect(GLfloat aspect){
	this->aspect = aspect;
    /* Create our projection matrix with a 45 degree field of view
     * a width to height ratio of 1 and view from .1 to 100 infront of us */
    perspective();
}

void Camera::move(GLfloat x, GLfloat y, GLfloat z){
	eye += x*center;
	update();
}

void Camera::forward(){
	QVector3D front = center;
	front.normalize();
	eye += speed*front;
	update();
}

void Camera::backward(){
	QVector3D front = center;
	front.normalize();
	eye -= speed*front;
	update();
}

void Camera::left(){
	QVector3D side = QVector3D::crossProduct ( center, up );
	side.normalize();
	eye -= speed * side;
	update();
}

void Camera::right(){
	QVector3D side = QVector3D::crossProduct ( center, up );
	side.normalize();
	eye += speed * side;
	update();
}

void Camera::setMouseLook(int mouseXrel, int mouseYrel){

	pitch -= mouseSensitivity * mouseXrel;
	yaw -= mouseSensitivity * mouseYrel;
	if (yaw > 89) yaw = 89;
	if (yaw < -89) yaw = -89;

	updateRotation();
	//printf("Yaw: %f Pitch: %f\n", yaw, pitch);
	update();

}

void Camera::setMouseZoom(int wheelX, int wheelY){
	cout << "Wheel\t" << wheelX << "\t" << wheelY << "\n";
}

void Camera::updateRotation(){

	QMatrix4x4 rotate = QMatrix4x4();

	rotate.rotate(pitch, 0, 1, 0);
	rotate.rotate(yaw, 1, 0, 0);

	center = rotate * defaultCenter;
	center.normalize();
	update();
}

void Camera::setParams(GLfloat fov, GLfloat nearz, GLfloat farz){
	this->fov = fov;
	this->nearz = nearz;
	this->farz = farz;
}

void Camera::update(){
	viewMatrix.setToIdentity();
	viewMatrix.lookAt(eye,center+eye,up);
	centerNode->setPosition(center+eye);
}

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void Camera::perspective()
{
	viewMatrix.setToIdentity();
	projectionMatrix.perspective(fov,aspect,nearz,farz);
}
