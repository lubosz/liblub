/*
 * Config.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: bmonkey
 */

#include "Config.h"
#include "Logger.h"
#include <QSize>
#include <QStringList>
#include <QFile>

Config::Config() {

}


Config::~Config() {
	// TODO Auto-generated destructor stub
}

void Config::load(const QString & fileName){
	QFile file(fileName);

	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument domDocument;
	domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn);

	QDomElement document = domDocument.documentElement().firstChildElement();
	while (!document.isNull()) {
		if (document.tagName() == "Config"){
			QDomElement option = document.firstChildElement();
			while (!option.isNull()) {
				appendOption(option);
				option = option.nextSiblingElement();
			}
		}
		document = document.nextSiblingElement();
	}
}

void Config::appendOption(const QDomElement & optionNode){
		string name = optionNode.attribute("name").toStdString();
		Logger::Instance().log("DEBUG", "Config", "Option name:"+name);

		if (optionNode.tagName() == "Int"){
		}else if (optionNode.tagName() == "String"){
		}else if (optionNode.tagName() == "Float"){
		}else if (optionNode.tagName() == "Bool"){
		}

	}
