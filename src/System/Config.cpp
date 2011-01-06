/*
 * Copyright © 2010 Lubosz Sarnecki
 * Config.cpp
 *
 *  Created on: Dec 20, 2010
 */
#include <QSize>
#include <QStringList>
#include <QFile>
#include <vector>
#include <string>
#include <typeinfo>
#include "System/Config.h"
#include "System/Logger.h"

Config::Config():XmlReader() {
  // Template fix

}

Config::~Config() {}

template<typename T>
vector<T> Config::getValues(string name, const vector<ConfigOption<T>> & config) {
    foreach(ConfigOption<T> configOption, config) {
            if (configOption.name == name)
                return configOption.optionVec;
    }
}

template<> vector<bool> Config::values<bool>(const string & name) {
  return getValues<bool>(name, bools);
}
template<> vector<int> Config::values<int>(const string & name) {
  return getValues<int>(name, ints);
}
template<> vector<string> Config::values<string>(const string & name) {
  return getValues<string>(name, strings);
}
template<> vector<float> Config::values<float>(const string & name) {
  return getValues<float>(name, floats);
}
template<> bool Config::value<bool>(const string & name) {
    return values<bool>(name)[0];
}
template<> string Config::value<string>(const string & name) {
    return values<string>(name)[0];
}
template<> int Config::value<int>(const string & name) {
    return values<int>(name)[0];
}
template<> float Config::value<float>(const string & name) {
    return values<float>(name)[0];
}

void Config::load(const QString & fileName) {
    QFile file(fileName);

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument domDocument;
    domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn);

    QDomElement document = domDocument.documentElement().firstChildElement();
    while (!document.isNull()) {
        if (document.tagName() == "Config") {
            QDomElement option = document.firstChildElement();
            while (!option.isNull()) {
                appendOption(option);
                option = option.nextSiblingElement();
            }
        }
        document = document.nextSiblingElement();
    }
}

void Config::appendOption(const QDomElement & optionNode) {
    string name = optionNode.attribute("name").toStdString();
    QString value = optionNode.attribute("value");
    Logger::Instance().log("DEBUG", "Config", "Option name:" + name);

    if (optionNode.tagName() == "Int") {
        vector<int> options = splitValues<int> (value);
        ints.push_back(ConfigOption<int> (name, options));
    } else if (optionNode.tagName() == "String") {
        vector<string> options = splitValues<string> (value);
        strings.push_back(ConfigOption<string> (name, options));
    } else if (optionNode.tagName() == "Float") {
        vector<float> options = splitValues<float> (value);
        floats.push_back(ConfigOption<float> (name, options));
    } else if (optionNode.tagName() == "Bool") {
        vector<bool> options = splitValues<bool> (value);
        bools.push_back(ConfigOption<bool> (name, options));
    }
}
