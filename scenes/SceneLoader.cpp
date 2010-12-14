#include <QFile>
#include <QStringList>

#include "Application.h"
#include "MengerSponge.h"

class SceneLoader: public Application {

public:


	SceneLoader() {
		QFile file("media/scenes/test.xml");

		QString errorStr;
		int errorLine;
		int errorColumn;
		QDomDocument domDocument;
		domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn);

		sceneXML = domDocument.documentElement();
//		if(sceneXML.tagName() == "Scene")
//			programName = sceneXML.attribute("name").toStdString();

		programName ="XML Loader";

	}

	QVector3D stringToVector3D(const QString& values) {

		vector<float> floats;
		foreach (QString value, values.split(","))
			{
				floats.push_back(value.toFloat());
			}
		return QVector3D(floats[0], floats[1], floats[2]);
	}

	void scene() {
		QDomElement document = sceneXML.firstChildElement();
		while (!document.isNull()) {

			if (document.tagName() == "Scene"){
				QDomElement scene = document.firstChildElement();
				while (!scene.isNull()) {
					QVector3D position, direction;
					string name;
					float scale;
					Material * material;
					Mesh * mesh;

					if (scene.hasAttribute("position"))
						position = stringToVector3D(scene.attribute("position"));
					if (scene.hasAttribute("direction"))
						direction = stringToVector3D(scene.attribute("direction"));
					if (scene.hasAttribute("name"))
						name = scene.attribute("name").toStdString();
					if (scene.hasAttribute("scale"))
						scale = scene.attribute("scale").toFloat();
					if (scene.hasAttribute("material"))
						material = new Simple(scene.attribute("material").toStdString());

					if (scene.tagName() == "Light") {
						SceneGraph::Instance().light =
								new Light(
										position,
										direction
								);
					} else if (scene.tagName() == "Object") {
						mesh = MeshFactory::load(scene.attribute("mesh").toStdString());
					}else if (scene.tagName() == "Procedural") {
						if(scene.attribute("type") == "Sponge"){
							MengerSponge * sponge = new MengerSponge(scene.attribute("recursion").toInt());
							mesh = sponge->getMesh();
						}else if(scene.attribute("type") == "Stars"){
							mesh = MeshFactory::stars(
									scene.attribute("resolution").toFloat(),
									scene.attribute("density").toFloat(),
									scene.attribute("randomness").toFloat(),
									scene.attribute("colorIntensity").toFloat()
							);
						}
					}else if (scene.tagName() == "MeshPlane") {
						SceneGraph::Instance().meshPlane(
								scene.attribute("mesh").toStdString(),
								scene.attribute("size").toFloat(),
								scene.attribute("step").toFloat(),
								{
										new PhongTexMat(scene.attribute("texture").toStdString())
								}
						);
					}

					if (scene.tagName() == "Object" || scene.tagName() == "Procedural")
					SceneGraph::Instance().addNode(
							new Node(
									name,
									position,
									scale,
									mesh,
									material
							)
					);
					scene = scene.nextSiblingElement();
				}
			}
			document = document.nextSiblingElement();
		}

	}
};

int main(int argc, char *argv[]) {
	SceneLoader().run();
}
