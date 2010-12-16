/*
 * SceneLoader.cpp
 *
 *  Created on: Dec 16, 2010
 *      Author: bmonkey
 */
#include <vector>
#include <QStringList>
#include <QFile>
#include "MeshFactory.h"
#include "SceneLoader.h"
#include "MengerSponge.h"

SceneLoader::SceneLoader(QString fileName) {
	QFile file(fileName);

	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument domDocument;
	domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn);

	sceneXML = domDocument.documentElement();

}

SceneLoader::~SceneLoader() {
	// TODO Auto-generated destructor stub
}


QVector3D SceneLoader::stringToVector3D(const QString& values) {

	vector<float> floats;
	foreach (QString value, values.split(","))
		{
			floats.push_back(value.toFloat());
		}
	return QVector3D(floats[0], floats[1], floats[2]);
}

void SceneLoader::appendNode(const QDomElement & node){
	QVector3D position, direction;
	string name;
	float scale;
	Material * material;
	Mesh * mesh;

	if (node.hasAttribute("position"))
		position = stringToVector3D(node.attribute("position"));
	if (node.hasAttribute("direction"))
		direction = stringToVector3D(node.attribute("direction"));
	if (node.hasAttribute("name"))
		name = node.attribute("name").toStdString();
	if (node.hasAttribute("scale"))
		scale = node.attribute("scale").toFloat();
	if (node.hasAttribute("material"))
		material = new Simple(node.attribute("material").toStdString());

	if (node.tagName() == "Light") {
		SceneGraph::Instance().light =
				new Light(
						position,
						direction
				);
	} else if (node.tagName() == "Object") {
		mesh = MeshFactory::load(node.attribute("mesh").toStdString());
	}else if (node.tagName() == "Procedural") {
		if(node.attribute("type") == "Sponge"){
			MengerSponge * sponge = new MengerSponge(node.attribute("recursion").toInt());
			mesh = sponge->getMesh();
		}else if(node.attribute("type") == "Stars"){
			mesh = MeshFactory::stars(
					node.attribute("resolution").toFloat(),
					node.attribute("density").toFloat(),
					node.attribute("randomness").toFloat(),
					node.attribute("colorIntensity").toFloat()
			);
		}
	}else if (node.tagName() == "MeshPlane") {
		SceneGraph::Instance().meshPlane(
				node.attribute("mesh").toStdString(),
				node.attribute("size").toFloat(),
				node.attribute("step").toFloat(),
				{
						new PhongTexMat(node.attribute("texture").toStdString())
				}
		);
	}

	if (node.tagName() == "Object" || node.tagName() == "Procedural")
	SceneGraph::Instance().addNode(
			new Node(
					name,
					position,
					scale,
					mesh,
					material
			)
	);
}

void SceneLoader::load(){
	QDomElement document = sceneXML.firstChildElement();
	while (!document.isNull()) {

		if (document.tagName() == "Programs"){
			QDomElement programs = document.firstChildElement();
			Logger::Instance().log("Programs");
			printf("foooooooo");
			while (!programs.isNull()) {
//				appendNode(scene);
				programs = programs.nextSiblingElement();
			}
		}else if (document.tagName() == "Materials"){
			QDomElement materials = document.firstChildElement();
			Logger::Instance().log("Materials");
			while (!materials.isNull()) {
//				appendNode(scene);
				materials = materials.nextSiblingElement();
			}
		}else if (document.tagName() == "Scene"){
			QDomElement scene = document.firstChildElement();
			while (!scene.isNull()) {
				appendNode(scene);
				scene = scene.nextSiblingElement();
			}
		}
		document = document.nextSiblingElement();
	}
}
