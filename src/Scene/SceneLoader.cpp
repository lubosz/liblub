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

vector<string> SceneLoader::splitFlags(QString values){
	vector<string> flags;
	foreach (QString value, values.split(","))
		{
			flags.push_back(value.trimmed().toStdString());
		}
	return flags;
}

vector<float> SceneLoader::splitUniform(QString values){
	vector<float> flags;
	foreach (QString value, values.split(","))
		{
			flags.push_back(value.toFloat());
		}
	return flags;
}

QVector3D SceneLoader::stringToVector3D(const QString& values) {

	vector<float> floats;
	foreach (QString value, values.split(","))
		{
			floats.push_back(value.toFloat());
		}
	return QVector3D(floats[0], floats[1], floats[2]);
}


void SceneLoader::appendProgram(const QDomElement & program){
	string name, shaderUrl;
	vector<string> flags;
	ShaderProgram shaderProgram;

	if (program.hasAttribute("name"))
		shaderProgram.name = program.attribute("name").toStdString();

	QDomElement programInfo = program.firstChildElement();
	while (!programInfo.isNull()) {
		if (programInfo.tagName() == "Shader"){
				shaderUrl = programInfo.attribute("url").toStdString();
				flags = splitFlags(programInfo.attribute("flags"));
				shaderProgram.attachVertFrag(shaderUrl,flags);
		}else if (programInfo.tagName() == "Uniform"){
			shaderProgram.uniforms.push_back(
					Uniform(
							programInfo.attribute("name").toStdString(),
							splitUniform(programInfo.attribute("value"))
					)
			);
		}
		programInfo = programInfo.nextSiblingElement();
	}
}

void SceneLoader::appendMaterial(const QDomElement & material){
	string name, program;
	vector<string> layerStrings;

	if (material.hasAttribute("name"))
		name = material.attribute("name").toStdString();
	if (material.hasAttribute("program"))
		name = material.attribute("program").toStdString();

	QDomElement layers = material.firstChildElement();
	while (!layers.isNull()) {
		if (layers.hasAttribute("texture"))
			layerStrings.push_back(layers.attribute("program").toStdString());
		layers = layers.nextSiblingElement();
	}
}

void SceneLoader::appendTexture(const QDomElement & texture){
	string name, url;

	if (texture.hasAttribute("name"))
		name = texture.attribute("name").toStdString();
	if (texture.hasAttribute("url"))
		name = texture.attribute("url").toStdString();
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
			while (!programs.isNull()) {
				appendProgram(programs);
				programs = programs.nextSiblingElement();
			}
		}else if (document.tagName() == "Materials"){
			QDomElement materials = document.firstChildElement();
			while (!materials.isNull()) {
				appendMaterial(materials);
				materials = materials.nextSiblingElement();
			}
		}else if (document.tagName() == "Scene"){
			QDomElement scene = document.firstChildElement();
			while (!scene.isNull()) {
				appendNode(scene);
				scene = scene.nextSiblingElement();
			}
		}else if (document.tagName() == "Textures"){
			QDomElement textures = document.firstChildElement();
			while (!textures.isNull()) {
				appendNode(textures);
				textures = textures.nextSiblingElement();
			}
		}
		document = document.nextSiblingElement();
	}
}
