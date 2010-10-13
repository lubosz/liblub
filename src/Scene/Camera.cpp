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
#include "MediaLayer.h"

Camera::Camera(){
	direction = QVector3D(0, 0, -1);
	position = QVector3D();
	aspect = float(MediaLayer::Instance().width)/float(MediaLayer::Instance().height);
	defaultValues();

	cout << "camera11111" << near << " "<< far << "\n\n\n\n\n\n";
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::forward(){
	QVector3D front = direction;
	front.normalize();
	position += speed*front;
	updateView();
}

void Camera::backward(){
	QVector3D front = direction;
	front.normalize();
	position -= speed*front;
	updateView();
}

void Camera::left(){
	QVector3D side = QVector3D::crossProduct ( direction, up );
	side.normalize();
	position -= speed * side;
	updateView();
}

void Camera::right(){
	QVector3D side = QVector3D::crossProduct ( direction, up );
	side.normalize();
	position += speed * side;
	updateView();
}

void Camera::setMouseLook(int mouseXrel, int mouseYrel){

	pitch -= mouseSensitivity * mouseXrel;
	yaw -= mouseSensitivity * mouseYrel;
	if (yaw > 89) yaw = 89;
	if (yaw < -89) yaw = -89;

	updateRotation();
}

void Camera::setMouseZoom(int wheelX, int wheelY){
	cout << "Wheel\t" << wheelX << "\t" << wheelY << "\n";
}

void Camera::updateRotation(){

	rotation.setToIdentity();
	rotation.rotate(pitch, 0, 1, 0);
	rotation.rotate(yaw, 1, 0, 0);
	direction = rotation * defaultCenter;
	direction.normalize();
	updateView();
}
