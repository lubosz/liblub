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
#include <QDir>
#include <QTextStream>

Config::Config():XmlReader() {

    string mediaPrefix = getMediaPrefix();

    addString("shaderDir", mediaPrefix + "shaders/");
    addString("textureDir", mediaPrefix +  "media/textures/");
    addString("meshDir", mediaPrefix + "media/meshes/");
    addString("sceneDir", mediaPrefix + "media/scenes/");

  QString configPath = QDir::homePath() + "/.liblub";

  if (!QDir(configPath).exists()) {
      LogInfo << "Creating Config Path" << configPath.toStdString();
      QDir().mkdir(configPath);
  }

  QFile * file = new QFile(configPath + "/config.xml");

  if (!file->exists()) {
      createConfigFile(file);
  }

  QString errorStr;
  int errorLine;
  int errorColumn;
  QDomDocument domDocument;
  domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn);

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

  file->close();
  delete file;
}

Config::~Config() {}

void Config::addString(const string& name, const string& option) {
    strings.push_back(ConfigOption<string> (name, option));
}

QStringList Config::getGLVersion() {
    FILE *in;
    char buff[512];

    if(!(in = popen("glxinfo | grep 'OpenGL version string'", "r"))){
        LogError << "Could not get GL Version from glxinfo";
    }

    QString glVersion;
    while(fgets(buff, sizeof(buff), in)!=NULL){
        glVersion = QString::fromStdString(buff);
    }
    pclose(in);

    QStringList glVersionSplit = glVersion.split("string: ");
    QString glVersionString = glVersionSplit[1];
    glVersionString.truncate(3);
    QStringList glVersionSplitFinal = glVersionString.split(".");
    return glVersionSplitFinal;
}

void Config::createConfigFile(QFile * file) {
    LogInfo << "Creating Config File" << file->fileName().toStdString();
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(file);

    QStringList glVersion = getGLVersion();

    out << "<?xml version='1.0' encoding='UTF-8'?>\n"
        << "<liblub>\n"
        << "    <Config>\n"
        << "        <Int name='Vsync' value='1' />\n"
        << "        <Int name='touchPadFix' value='0' />\n"
        << "        <String name='suffixes' value='_RT, _LF, _DN,_UP, _FR, _BK' />\n"
        << "        <Float name='backgroundColor' value='0.0,0.0,0.0' />\n"
        << "        <Float name='inputSpeedFast' value='0.2' />\n"
        << "        <Float name='inputSpeedSlow' value='0.03' />\n"
        << "        <Float name='mouseSensitivity' value='0.1' />\n"
        << "        <Int name='GLcontext' value='" << glVersion[0] << "," << glVersion[1] << "' />\n"
        << "    </Config>\n"
        << "</liblub>";
    file->close();
}

string Config::getMediaPrefix() {
    QStringList mediaPrefixes = QStringList() << "/usr/share/liblub/" << "/usr/local/share/liblub/" << "";
    string mediaPrefix = "None";
    QString allPrefixes = "";

    foreach(QString prefix, mediaPrefixes) {
        allPrefixes += prefix + "media, ";
        if (QDir(prefix + "media").exists())
            mediaPrefix = prefix.toStdString();
    }

    if (mediaPrefix == "None") {
        LogError << "Media directory not found.";
        LogError << "Tried" << allPrefixes.toStdString();
        exit(0);
    }

    LogInfo << "Media Prefix:" << mediaPrefix;
    return mediaPrefix;
}

template<typename T>
vector<T> Config::getValues(const string& name, const vector<ConfigOption<T>> & config) {
    foreach(ConfigOption<T> configOption, config) {
            if (configOption.name == name)
                return configOption.optionVec;
    }
    LogError << "Config not found" << name;
    return vector<T>();
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

void Config::appendOption(const QDomElement & optionNode) {
    string name = optionNode.attribute("name").toStdString();
    QString value = optionNode.attribute("value");
    LogDebug << "Option name:" << name;

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
