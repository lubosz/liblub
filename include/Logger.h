/*
 * Logger.h
 *
 *  Created on: Dec 9, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "Singleton.h"

using namespace std;

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
