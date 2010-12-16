/*
 * SceneLoader.h
 *
 *  Created on: Dec 16, 2010
 *      Author: bmonkey
 */

#pragma once

#include <QVector3D>
#include <QDomElement>


class SceneLoader {
public:
	SceneLoader(QString fileName);
	virtual ~SceneLoader();

	void load();

	QDomElement sceneXML;

	QVector3D stringToVector3D(const QString& values);
	void appendNode(const QDomElement & node);
};
