/*
 * SceneLoader.h
 *
 *  Created on: Dec 16, 2010
 *      Author: bmonkey
 */

#pragma once

#include <QVector3D>
#include <QDomElement>
#include "ShaderProgram.h"


class SceneLoader {
public:
	SceneLoader(QString fileName);
	virtual ~SceneLoader();

	void load();

	QDomElement sceneXML;

	QVector3D stringToVector3D(const QString& values);
	void appendNode(const QDomElement & node);
	void appendMaterial(const QDomElement & material);
	void appendProgram(const QDomElement & program);
	void appendTexture(const QDomElement & texture);
	vector<string> splitFlags(QString values);
	vector<float> splitUniform(QString values);

	QMap<string,ShaderProgram*> shaderPrograms;
};
