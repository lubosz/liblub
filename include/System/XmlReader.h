/*
 * Copyright © 2010 Lubosz Sarnecki
 * XmlReader.h
 *
 *  Created on: Jan 4, 2011
 */

#pragma once

#include <QString>
#include <QStringList>
#include <vector>
#include "common/Singleton.h"

using std::vector;
using std::string;

class XmlReader {
 public:
	template<typename T> T pushValue(const QString& value);
	template<typename T>
	vector<T> splitValues(QString values) {
	    vector<T> flags;
	    foreach(QString value, values.split(","))
	            flags.push_back(pushValue<T> (value));
	    return flags;
	}
};



