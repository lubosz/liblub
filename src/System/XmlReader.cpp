/*
 * Copyright © 2010 Lubosz Sarnecki
 * XmlReader.cpp
 *
 *  Created on: Jan 4, 2011
 */
#include <string>
#include <QStringList>
#include "XmlReader.h"

using std::string;

template<> int XmlReader::pushValue<int>(const QString& value) {
    return value.toInt();
}

template<> string XmlReader::pushValue<string>(const QString& value) {
    return value.trimmed().toStdString();
}

template<> float XmlReader::pushValue<float>(const QString& value) {
    return value.toFloat();
}

template<typename T>
vector<T> XmlReader::splitValues(QString values) {
    vector<T> flags;
    foreach(QString value, values.split(","))
            flags.push_back(pushValue<T> (value));
    return flags;
}
