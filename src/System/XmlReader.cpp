/*
 * Copyright © 2010 Lubosz Sarnecki
 * XmlReader.cpp
 *
 *  Created on: Jan 4, 2011
 */

#include <string>
#include <vector>
#include "System/XmlReader.h"

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

template<> bool XmlReader::pushValue<bool>(const QString& value) {
    if (value == "true")
        return true;
    else
        return false;
}
