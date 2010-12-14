#include "Application.h"
#include <QFile>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <iostream>
#include <QStringList>
class SceneLoader: public Application {

public:

	QVector3D stringToVector3D(const QString& values) {

		vector<float> floats;
		foreach (QString value, values.split(","))
			{
				floats.push_back(value.toFloat());
			}
		return QVector3D(floats[0], floats[1], floats[2]);
	}

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

	void scene() {
		QDomElement child = sceneXML.firstChildElement();
		while (!child.isNull()) {
			if (child.tagName() == "Light") {

				SceneGraph::Instance().light = new Light(stringToVector3D(
						child.attribute("position")), stringToVector3D(
						child.attribute("direction")));
			} else if (child.tagName() == "Object") {
				SceneGraph::Instance().addNode(new Node(
						child.attribute("name").toStdString(),
						stringToVector3D(child.attribute("position")),
						child.attribute("scale").toFloat(), MeshFactory::load(
								child.attribute("mesh").toStdString()),
						new Simple(child.attribute("material").toStdString())
				//new PhongTexMat("Paper_Texture_by_Spiteful_Pie_Stock.jpg")
						));
			}else if (child.tagName() == "Object") {

			}
			child = child.nextSiblingElement();
		}

	}
};

int main(int argc, char *argv[]) {
	/*
	 QDomElement root = domDocument.documentElement();
	 printf("%s\n", qPrintable(root.tagName()));

	 QDomElement child = root.firstChildElement();
	 while (!child.isNull()) {
	 printf("\t%s\n", qPrintable(child.tagName()));
	 printf("\t\t%s\n", qPrintable(child.text()));
	 child = child.nextSiblingElement();

	 QDomElement child2 = child.firstChildElement();
	 while (!child2.isNull()) {
	 printf("\t\t%s\n", qPrintable(child2.tagName()));
	 printf("\t\t\t%s\n", qPrintable(child2.text()));
	 child2 = child2.nextSiblingElement();
	 QDomElement child3 = child2.firstChildElement();
	 while (!child3.isNull()) {
	 printf("\t\t\t%s\n", qPrintable(child3.tagName()));
	 printf("\t\t\t\t%s\n", qPrintable(child3.text()));

	 child3 = child3.nextSiblingElement();
	 }
	 }

	 }
	 */
	//    foreach(QDomNode child, root.childNodes()){
	//    	printf("Attrib: %s\n", qPrintable(child()));
	//    }
	SceneLoader().run();

}
