/*
 * Copyright © 2010 Lubosz Sarnecki
 * XmlReader.h
 *
 *  Created on: Jan 4, 2011
 */

#pragma once

#include <QString>
#include <vector>
#include "Singleton.h"

using namespace std;

class XmlReader: public Singleton<XmlReader> {
 public:
	XmlReader();
	virtual ~XmlReader();
	template<typename T> T pushValue(const QString& value);
	template<typename T>  vector<T> splitValues(QString values);
};



