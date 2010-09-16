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

//#include "utils.h"

#include "Camera.h"
#include "common.h"

using namespace std;



Camera::Camera() {
	projectionMatrix = QMatrix4x4();
	viewMatrix = QMatrix4x4();
    x = 0;
    y = 0;
    z = -2;

    yaw, pitch, roll = 0;
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
	this->x+=x;
	this->y+=y;
	this->z+=z;
	update();
}

void Camera::rotate(GLfloat yaw, GLfloat pitch, GLfloat roll){
	this->yaw+=yaw;
	this->pitch+=pitch;
	this->roll+=roll;
	update();
}

void Camera::setParams(GLfloat fov, GLfloat nearz, GLfloat farz){
	this->fov = fov;
	this->nearz = nearz;
	this->farz = farz;
}

void Camera::update(){
	viewMatrix.setToIdentity();
	viewMatrix.lookAt({x,y,z},{x+yaw,y+pitch,z-2},{0,1,0});
}

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void Camera::perspective()
{
	viewMatrix.setToIdentity();
	projectionMatrix.perspective(fov,aspect,nearz,farz);
}
