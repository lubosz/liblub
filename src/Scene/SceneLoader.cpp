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
#include "Geometry.h"
#include <typeinfo>

SceneLoader::SceneLoader(const QString & fileName) :fileName(fileName) {}

SceneLoader::~SceneLoader() {
	// TODO Auto-generated destructor stub
}

template<> int SceneLoader::pushValue<int>( QString& value ){
	return value.toInt();
}

template<> string SceneLoader::pushValue<string>( QString& value ){
	return value.trimmed().toStdString();
}

template<> float SceneLoader::pushValue<float>( QString& value ){
	return value.toFloat();
}

template <typename T>
vector<T> SceneLoader::splitValues(QString values){
	vector<T> flags;
	T * foo;
	foreach (QString value, values.split(","))
		flags.push_back(pushValue<T>(value));
	return flags;
}

QVector3D SceneLoader::stringToVector3D(const QString& values) {

	vector<float> floats = splitValues<float>(values);
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
						flags = splitValues<string>(programInfo.attribute("flags"));
						program->attachVertFrag(shaderUrl,flags);
					}else{
						program->attachVertFrag(shaderUrl);
					}
				}
		}else if (programInfo.tagName() == "Uniform"){
			program->uniforms.push_back(
					Uniform<float>(
							programInfo.attribute("name").toStdString(),
							splitValues<float>(programInfo.attribute("value"))
					)
			);
		}else if (programInfo.tagName() == "Uniformi"){
			program->uniformsi.push_back(
					Uniform<int>(
							programInfo.attribute("name").toStdString(),
							splitValues<int>(programInfo.attribute("value"))
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
	//todo: init shadow map texutre only when needed
	material->addTexture(RenderEngine::Instance().shadowSequence->renderPasses[0]->targetTexture);
	material->shaderProgram->use();
	material->samplerUniforms();
	materials.insert(name,material);
}

void SceneLoader::appendTexture(const QDomElement & textureNode){
	string name = textureNode.attribute("name").toStdString();
	Logger::Instance().log("DEBUG", "SceneLoader", "Texture name:"+name);
	Texture * texture;
	if (textureNode.tagName() == "Texture"){
		texture = TextureFactory::Instance().load(textureNode.attribute("url").toStdString(), name);
	}else if (textureNode.tagName() == "TextureCube"){
		texture = TextureFactory::Instance().loadCubeMap(textureNode.attribute("url").toStdString(), name);
	}
	textures.insert(
			name,
			texture
	);
}

void SceneLoader::appendMesh(const QDomElement & meshNode){
	string name = meshNode.attribute("name").toStdString();
	Logger::Instance().log("DEBUG", "SceneLoader", "Mesh name:"+name);
	Mesh *mesh;

	if (meshNode.tagName() == "File") {
		string meshUrl = meshNode.attribute("url").toStdString();
		if(meshNode.hasAttribute("drawType")){
			GLint drawType;
			string drawTypeString = meshNode.attribute("drawType").toStdString();

			if(drawTypeString == "PATCHES")
				drawType = GL_PATCHES;
			else if(drawTypeString == "POINTS")
				drawType = GL_POINTS;
			else if(drawTypeString == "TRIANGLE_STRIP")
				drawType = GL_TRIANGLE_STRIP;
			else if(drawTypeString == "TRIANGLES")
				drawType = GL_TRIANGLES;
			else if(drawTypeString == "LINES")
				drawType = GL_LINES;

			mesh = MeshFactory::load(meshUrl,drawType);
		}else{
			mesh = MeshFactory::load(meshUrl);
		}
	}else if (meshNode.tagName() == "Procedural") {

			if(meshNode.attribute("type") == "Sponge"){
				MengerSponge * sponge = new MengerSponge(meshNode.attribute("recursion").toInt());
				mesh = sponge->getMesh();
			}else if(meshNode.attribute("type") == "Stars"){
				mesh = MeshFactory::stars(
						meshNode.attribute("resolution").toFloat(),
						meshNode.attribute("density").toFloat(),
						meshNode.attribute("randomness").toFloat(),
						meshNode.attribute("colorIntensity").toFloat()
				);
			}else if(meshNode.attribute("type") == "Tess"){
				mesh = Geometry::makePlaneTess();
			}else if(meshNode.attribute("type") == "Spiral"){
				mesh = Geometry::makeSpiral(
						meshNode.attribute("resolution").toFloat(),
						meshNode.attribute("density").toFloat(),
						meshNode.attribute("randomness").toFloat()
				);
			}
	}


	meshes.insert(
			name,
			mesh

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

		string meshName = objectNode.attribute("mesh").toStdString();
		if(meshes.count(meshName) > 0)
			mesh = meshes.value(meshName);
		else
			Logger::Instance().log("ERROR", "Mesh Not Found", meshName);


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
	}else if (objectNode.tagName() == "ObjectPlane") {
		SceneGraph::Instance().meshPlane(
				objectNode.attribute("mesh").toStdString(),
				objectNode.attribute("size").toFloat(),
				objectNode.attribute("step").toFloat(),
				{
						new PhongTexMat(objectNode.attribute("texture").toStdString())
				}
		);
	}
}

void SceneLoader::load(){
	load(fileName);
}

void SceneLoader::load(const QString & fileName){
	QFile file(QString(sceneDir.c_str()) + fileName);

	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument domDocument;
	domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn);

	sceneXML = domDocument.documentElement();

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
		}else if (document.tagName() == "Meshes"){
			QDomElement meshes = document.firstChildElement();
			while (!meshes.isNull()) {
				appendMesh(meshes);
				meshes = meshes.nextSiblingElement();
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
		}else if (document.tagName() == "include"){
			load(document.attribute("url"));
		}
		document = document.nextSiblingElement();
	}
}
