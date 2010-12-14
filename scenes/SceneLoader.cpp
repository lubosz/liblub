#include "Application.h"
#include <QFile>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <iostream>
#include <QStringList>
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

		programName = sceneXML.attribute("name").toStdString();

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
		QDomElement child = sceneXML.firstChildElement();
		while (!child.isNull()) {
			if (child.tagName() == "Light") {

				SceneGraph::Instance().light =
						new Light(
								stringToVector3D(child.attribute("position")),
								stringToVector3D(child.attribute("direction"))
						);

			} else if (child.tagName() == "Object") {

				SceneGraph::Instance().addNode(
						new Node(
							child.attribute("name").toStdString(),
							stringToVector3D(child.attribute("position")),
							child.attribute("scale").toFloat(),
							MeshFactory::load(child.attribute("mesh").toStdString()),
							new Simple(child.attribute("material").toStdString())
						)
				);

			}else if (child.tagName() == "Procedural") {

				if(child.attribute("type") == "Sponge"){
					MengerSponge * sponge = new MengerSponge(child.attribute("recursion").toInt());
				    SceneGraph::Instance().addNode(
				    		new Node(
				    				child.attribute("name").toStdString(),
				    				stringToVector3D(child.attribute("position")),
				    				child.attribute("scale").toFloat(),
				    				sponge->getMesh(),
				    				new Simple(child.attribute("material").toStdString())
				    		)
				    );

				}else if(child.attribute("type") == "Stars"){
					SceneGraph::Instance().addNode(
							new Node(
									child.attribute("name").toStdString(),
									stringToVector3D(child.attribute("position")),
									child.attribute("scale").toFloat(),
									MeshFactory::stars(
											child.attribute("resolution").toFloat(),
											child.attribute("density").toFloat(),
											child.attribute("randomness").toFloat(),
											child.attribute("colorIntensity").toFloat()
									),
									new Simple(child.attribute("material").toStdString())
							)
					);
				}

			}else if (child.tagName() == "MeshPlane") {
				SceneGraph::Instance().meshPlane("monkey.blend", child.attribute("size").toFloat(), child.attribute("step").toFloat(), {new PhongTexMat(child.attribute("texture").toStdString())});
			}

			child = child.nextSiblingElement();
		}

	}
};

int main(int argc, char *argv[]) {
	SceneLoader().run();
}
