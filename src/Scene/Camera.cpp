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
	projectionMatrix = new Matrix();
    x = 0;
    y = 0;
    z = 0;

    yaw, pitch, roll = 0;
}

Matrix * Camera::getProjectionmatrix()
{
    return projectionMatrix;
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
}

void Camera::rotate(GLfloat yaw, GLfloat pitch, GLfloat roll){
	this->yaw+=yaw;
	this->pitch+=pitch;
	this->roll+=roll;
}

void Camera::setParams(GLfloat fov, GLfloat nearz, GLfloat farz){
	this->fov = fov;
	this->nearz = nearz;
	this->farz = farz;
}

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void Camera::perspective()
{
    GLfloat range;

    range = tan(fov * 0.00872664625) * nearz; /* 0.00872664625 = PI/360 */

    //projectionmatrix[0] = (2 * nearz) / ((range * aspect) - (-range * aspect));
    projectionMatrix->at(0) = nearz / (range * aspect);
    //projectionmatrix[0] = nearz / (range * aspect);

    //projectionmatrix[5] = (2 * nearz) / (2 * range);
    projectionMatrix->at(5) = nearz / range;

    projectionMatrix->at(10) = -(farz + nearz) / (farz - nearz);

    projectionMatrix->at(11) = -1;
    projectionMatrix->at(14) = -(2 * farz * nearz) / (farz - nearz);

    //projectionmatrix[11] = -(2 * farz * nearz) / (farz - nearz);
    //projectionmatrix[14] = -1;

}
