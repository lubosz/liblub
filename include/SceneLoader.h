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

	//	template<class T> void f( int, T, double ); // (c)
	//	template<class T> void f( T* );             // (d)
	//
	//	// A full specialization of (b) for int


//		template<class T> static T pushValue(QString& value);
//
//		template<> static int pushValue(QString& value){
//			return value.toInt();
//		}

class SceneLoader {
public:
	SceneLoader(const QString & fileName);
	virtual ~SceneLoader();

	void load(const QString & fileName);
	void load();

	QDomElement sceneXML;
	QString fileName;

	template<typename T> T pushValue( QString& value );
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
