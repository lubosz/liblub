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
#include "Texture.h"
#include "Material.h"


class SceneLoader {
public:
	SceneLoader(QString fileName);
	virtual ~SceneLoader();

	void load();

	QDomElement sceneXML;

	QVector3D stringToVector3D(const QString& values);
	void appendObject(const QDomElement & objectNode);
	void appendMaterial(const QDomElement & materialNode);
	void appendProgram(const QDomElement & programNode);
	void appendTexture(const QDomElement & textureNode);
	vector<string> splitFlags(QString values);
	vector<float> splitUniform(QString values);
	vector<int> splitUniformi(QString values);

	QMap<string,ShaderProgram*> shaderPrograms;
	QMap<string,Texture*> textures;
	QMap<string,Material*> materials;
};
