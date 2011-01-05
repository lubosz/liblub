/*
 * Copyright © 2010 Lubosz Sarnecki
 * Logger.h
 *
 *  Created on: Dec 9, 2010
 */

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "Singleton.h"

using std::stringstream;
using std::string;
using std::vector;

class Logger : public Singleton<Logger> {
 public:
	Logger();
	virtual ~Logger();

	stringstream message;

	vector<string> blacklist;

	void log(string type);
	void log(string type, string name);
	void clear();
	void log(string type, string name, string say);
	string bashColor(string message, string color);
	string composeColor(int background, int foreground);
};
