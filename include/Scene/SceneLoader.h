/*
 * Copyright © 2010 Lubosz Sarnecki
 * SceneLoader.h
 *
 *  Created on: Dec 16, 2010
 */

#pragma once

#include <QVector3D>
#include <QDomElement>
#include "Material/ShaderProgram.h"
#include "Material/Texture.h"
#include "Material/Material.h"
#include "Mesh/Mesh.h"
#include "System/XmlReader.h"


class SceneLoader : public XmlReader {
 public:
	explicit SceneLoader(const QString & fileName);
	virtual ~SceneLoader();

	void load(const QString & fileName);
	void load();

	QDomElement sceneXML;
	QString fileName;
	template<typename T> vector<T> splitValues(QString values);

	QVector3D stringToVector3D(const QString& values);
	void appendObject(const QDomElement & objectNode);
	void appendMaterial(const QDomElement & materialNode);
	void appendProgram(const QDomElement & programNode);
	void appendTexture(const QDomElement & textureNode);
	void appendMesh(const QDomElement & meshNode);

	QMap<string, ShaderProgram*> shaderPrograms;
	QMap<string, Texture*> textures;
	QMap<string, Material*> materials;
	QMap<string, Mesh*> meshes;
};
