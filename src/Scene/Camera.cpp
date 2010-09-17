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

using namespace std;

Camera::Camera() {
	projectionMatrix = QMatrix4x4();
	viewMatrix = QMatrix4x4();
	eye = QVector3D();
	center = QVector3D(0,0,-2);
    yaw, pitch, roll = 0;
    speed = .1;
    mouseSensitivity = 1.0/500.0;
}

QMatrix4x4 Camera::getProjection() const
{
    return projectionMatrix;
}

QMatrix4x4 Camera::getView() const
{
    return viewMatrix;
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
	//eye += QVector3D(x,y,z);
	eye += x*center;
	update();
}

void Camera::forward(){
	eye += speed*center;
	update();
}

void Camera::backward(){
	eye -= speed*center;
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
	QVector3D side = QVector3D::crossProduct ( center, up );
	side.normalize();

	center += mouseSensitivity * mouseXrel * side;
	center -= mouseSensitivity * mouseYrel * up;

	update();

	/*
		cout
			<< "Center\t" << center.x() << "\t" << center.y() << "\t" << center.z() << "\n"
			<< "Eye\t" << eye.x() << "\t" << eye.y() << "\t" << eye.z() << "\n"
			<< "Mouse\t" << mouseX << "\t" << mouseY << "\n"
			<< "MouseRel\t" << mouseXrel << "\t" << mouseYrel << "\n";
	*/
}

void Camera::rotate(GLfloat yaw, GLfloat pitch, GLfloat roll){
	this->yaw=yaw;
	this->pitch=pitch;
	this->roll=roll;
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
}

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void Camera::perspective()
{
	viewMatrix.setToIdentity();
	projectionMatrix.perspective(fov,aspect,nearz,farz);
}
