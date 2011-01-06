/*
 * Copyright © 2010 Lubosz Sarnecki
 * XmlReader.cpp
 *
 *  Created on: Jan 4, 2011
 */
#include <QStringList>
#include <string>
#include <vector>
#include "System/XmlReader.h"

using std::string;

XmlReader::XmlReader(){
  //TODO: TEMPLATE HACK
  QString foo = "";
  splitValues<string>(foo);
  splitValues<int>(foo);
  splitValues<float>(foo);
  splitValues<bool>(foo);
}

template<> int XmlReader::pushValue<int>(const QString& value) {
    return value.toInt();
}

template<> string XmlReader::pushValue<string>(const QString& value) {
    return value.trimmed().toStdString();
}

template<> float XmlReader::pushValue<float>(const QString& value) {
    return value.toFloat();
}

template<> bool XmlReader::pushValue<bool>(const QString& value) {
    if (value == "true")
        return true;
    else
        return false;
}

template<typename T>
vector<T> XmlReader::splitValues(QString values) {
    vector<T> flags;
    foreach(QString value, values.split(","))
            flags.push_back(pushValue<T> (value));
    return flags;
}
