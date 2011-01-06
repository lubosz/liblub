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
#include "System/Config.h"
#include "System/Logger.h"

Config::Config():XmlReader() {}

Config::~Config() {}

template<> vector<bool> Config::values<bool>(string name) {
    foreach(ConfigOption<bool> configOption, bools) {
            if (configOption.name == name)
                return configOption.optionVec;
    }
}

template<> vector<int> Config::values<int>(string name) {
    foreach(ConfigOption<int> configOption, ints) {
            if (configOption.name == name)
                return configOption.optionVec;
    }
}

template<> vector<string> Config::values<string>(string name) {
    foreach(ConfigOption<string> configOption, strings) {
            if (configOption.name == name)
                return configOption.optionVec;
        }
}

template<> vector<float> Config::values<float>(string name) {
    foreach(ConfigOption<float> configOption, floats) {
            if (configOption.name == name)
                return configOption.optionVec;
        }
}

template<> bool Config::value<bool>(string name) {
    foreach(ConfigOption<bool> configOption, bools) {
            if (configOption.name == name)
                return configOption.optionVec[0];
        }
}

template<> int Config::value<int>(string name) {
    foreach(ConfigOption<int> configOption, ints) {
            if (configOption.name == name)
                return configOption.optionVec[0];
        }
}

template<> string Config::value<string>(string name) {
    foreach(ConfigOption<string> configOption, strings) {
            if (configOption.name == name)
                return configOption.optionVec[0];
        }
}

template<> float Config::value<float>(string name) {
    foreach(ConfigOption<float> configOption, floats) {
            if (configOption.name == name)
                return configOption.optionVec[0];
        }
}

template<typename T>
vector<T> Config::splitValues(QString values) {
    vector<T> flags;
    foreach(QString value, values.split(","))
            flags.push_back(pushValue<T> (value));
    return flags;
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
