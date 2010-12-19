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

vector<int> SceneLoader::splitUniformi(QString values){
	vector<int> flags;
	foreach (QString value, values.split(","))
		{
			flags.push_back(value.toInt());
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


void SceneLoader::appendProgram(const QDomElement & programNode){
	string name, shaderUrl;
	vector<string> flags;
	ShaderProgram * program = new ShaderProgram();

	if (programNode.hasAttribute("name"))
		name = programNode.attribute("name").toStdString();

	QDomElement programInfo = programNode.firstChildElement();
	while (!programInfo.isNull()) {
		if (programInfo.tagName() == "Shader"){
				shaderUrl = programInfo.attribute("url").toStdString();
				if(programInfo.hasAttribute("type")){
					QString shaderType = programInfo.attribute("type");
					if(shaderType =="VERTEX_SHADER")
						program->attachShader(shaderUrl, GL_VERTEX_SHADER);
					else if(shaderType =="GEOMETRY_SHADER")
						program->attachShader(shaderUrl, GL_GEOMETRY_SHADER);
					else if(shaderType =="FRAGMENT_SHADER")
						program->attachShader(shaderUrl, GL_FRAGMENT_SHADER);
					else if(shaderType =="CONTROL_SHADER")
						program->attachShader(shaderUrl, GL_TESS_CONTROL_SHADER);
					else if(shaderType =="EVALUATION_SHADER")
						program->attachShader(shaderUrl, GL_TESS_EVALUATION_SHADER);
				}else{

					if (programInfo.hasAttribute("flags")){
						flags = splitFlags(programInfo.attribute("flags"));
						program->attachVertFrag(shaderUrl,flags);
					}else{
						program->attachVertFrag(shaderUrl);
					}
				}
		}else if (programInfo.tagName() == "Uniform"){
			program->uniforms.push_back(
					Uniform(
							programInfo.attribute("name").toStdString(),
							splitUniform(programInfo.attribute("value"))
					)
			);
		}else if (programInfo.tagName() == "Uniformi"){
			program->uniformsi.push_back(
					Uniformi(
							programInfo.attribute("name").toStdString(),
							splitUniformi(programInfo.attribute("value"))
					)
			);
		}
		programInfo = programInfo.nextSiblingElement();
	}
	program->init();
	shaderPrograms.insert(name, program);
}

void SceneLoader::appendMaterial(const QDomElement & materialNode){
	string name, program;
	vector<string> layerStrings;
	Material * material = new EmptyMat();

	if (materialNode.hasAttribute("name"))
		name = materialNode.attribute("name").toStdString();
	if (materialNode.hasAttribute("program")){
		program = materialNode.attribute("program").toStdString();
		if(shaderPrograms.count(program) > 0)
			material->shaderProgram = shaderPrograms.value(program);
		else
			Logger::Instance().log("ERROR", "Program Not Found", program);
	}else
		Logger::Instance().log("Error","NO SHADER PROGRAM");

	QDomElement layers = materialNode.firstChildElement();
	while (!layers.isNull()) {
		if (layers.hasAttribute("texture")){
			Texture * texture = textures.value(layers.attribute("texture").toStdString());
			if(textures.count(layers.attribute("texture").toStdString()) == 0)
				Logger::Instance().log("ERROR", "Scene Loader", "Texture " + layers.attribute("texture").toStdString()+" not found.");
			//todo: uniform name reset
			Logger::Instance().log("DEBUG", "SceneLoader",layers.attribute("sampler").toStdString());
			texture->name = layers.attribute("sampler").toStdString();
			material->addTexture(texture);
		}
		layers = layers.nextSiblingElement();
	}
	material->shaderProgram->use();
	material->bindTextures();
	materials.insert(name,material);
}

void SceneLoader::appendTexture(const QDomElement & textureNode){
	string name = textureNode.attribute("name").toStdString();
	Logger::Instance().log("DEBUG", "SceneLoader", "Texture name:"+name);
	textures.insert(
			name,
			TextureFactory::Instance().load(textureNode.attribute("url").toStdString(), name)

	);
}

void SceneLoader::appendObject(const QDomElement & objectNode){
	QVector3D position, direction;
	string name;
	float scale;
	Material * material;
	Mesh * mesh;

	if (objectNode.hasAttribute("position"))
		position = stringToVector3D(objectNode.attribute("position"));
	if (objectNode.hasAttribute("direction"))
		direction = stringToVector3D(objectNode.attribute("direction"));
	if (objectNode.hasAttribute("name"))
		name = objectNode.attribute("name").toStdString();
	if (objectNode.hasAttribute("scale"))
		scale = objectNode.attribute("scale").toFloat();
	if (objectNode.hasAttribute("material")){
		string materialName = objectNode.attribute("material").toStdString();
		if(materials.count(materialName) > 0)
			material = materials.value(materialName);
		else
			Logger::Instance().log("ERROR", "Material Not Found", materialName);
	}
	if (objectNode.tagName() == "Light") {
		SceneGraph::Instance().light =
				new Light(
						position,
						direction
				);
	} else if (objectNode.tagName() == "Object") {
		mesh = MeshFactory::load(objectNode.attribute("mesh").toStdString());
	}else if (objectNode.tagName() == "Procedural") {
		if(objectNode.attribute("type") == "Sponge"){
			MengerSponge * sponge = new MengerSponge(objectNode.attribute("recursion").toInt());
			mesh = sponge->getMesh();
		}else if(objectNode.attribute("type") == "Stars"){
			mesh = MeshFactory::stars(
					objectNode.attribute("resolution").toFloat(),
					objectNode.attribute("density").toFloat(),
					objectNode.attribute("randomness").toFloat(),
					objectNode.attribute("colorIntensity").toFloat()
			);
		}
	}else if (objectNode.tagName() == "MeshPlane") {
		SceneGraph::Instance().meshPlane(
				objectNode.attribute("mesh").toStdString(),
				objectNode.attribute("size").toFloat(),
				objectNode.attribute("step").toFloat(),
				{
						new PhongTexMat(objectNode.attribute("texture").toStdString())
				}
		);
	}

	if (objectNode.tagName() == "Object" || objectNode.tagName() == "Procedural"){
		Node * node = new Node(
				name,
				position,
				scale,
				mesh,
				material
		);
		if(objectNode.attribute("receive_shadows").contains("true", Qt::CaseInsensitive))
			node->setReceiveShadows(true);
		else if(objectNode.attribute("receive_shadows").contains("false", Qt::CaseInsensitive))
			node->setReceiveShadows(false);

		if(objectNode.attribute("cast_shadows").contains("true", Qt::CaseInsensitive))
			node->setCastShadows(true);
		else if(objectNode.attribute("cast_shadows").contains("false", Qt::CaseInsensitive))
			node->setCastShadows(false);

	SceneGraph::Instance().addNode(node);
	}
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
		}else if (document.tagName() == "Textures"){
			QDomElement textures = document.firstChildElement();
			while (!textures.isNull()) {
				appendTexture(textures);
				textures = textures.nextSiblingElement();
			}
		}
		else if (document.tagName() == "Materials"){
			QDomElement materials = document.firstChildElement();
			while (!materials.isNull()) {
				appendMaterial(materials);
				materials = materials.nextSiblingElement();
			}
		}else if (document.tagName() == "Scene"){
			QDomElement scene = document.firstChildElement();
			while (!scene.isNull()) {
				appendObject(scene);
				scene = scene.nextSiblingElement();
			}
		}
		document = document.nextSiblingElement();
	}
}
