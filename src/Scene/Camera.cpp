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

using namespace std;

Camera::Camera() {
	projectionMatrix = QMatrix4x4();
	viewMatrix = QMatrix4x4();
	eye = QVector3D();
	center = QVector3D(0,0,-5);
    yaw, pitch, roll = 0;
    speed = .1;
    mouseSensitivity = 1.0/100.0;
    centerNode = new Node("LookAt",center, .1, MeshFactory::Instance().cube(),new WhiteMat());
    SceneGraph::Instance().addNode(centerNode);
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

void Camera::setMouseLookInverseVP(int screenX, int screenY)
{
	QMatrix4x4 inverseVP = (projectionMatrix * viewMatrix).inverted();

	qreal nx = (2.0f * screenX) - 1.0f;
	qreal ny = 1.0f - (2.0f * screenY);
	QVector3D nearPoint(nx, ny, -1.f);
	// Use midPoint rather than far point to avoid issues with infinite projection
	QVector3D midPoint (nx, ny,  0.0f);

	// Get ray origin and ray target on near plane in world space
	QVector3D rayOrigin, rayTarget;

	rayOrigin = inverseVP * nearPoint;
	rayTarget = inverseVP * midPoint;

	QVector3D rayDirection = rayTarget - rayOrigin;
	rayDirection.normalize();

	//center = rayDirection;
	center = rayTarget-eye;
	update();

}

void Camera::setMouseZoom(int wheelX, int wheelY){
	//update();
	cout << "Wheel\t" << wheelX << "\t" << wheelY << "\n";

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
	centerNode->setPosition(center+eye);
}

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void Camera::perspective()
{
	viewMatrix.setToIdentity();
	projectionMatrix.perspective(fov,aspect,nearz,farz);
}
