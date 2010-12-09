/*
 * Logger.h
 *
 *  Created on: Dec 9, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <sstream>
#include "Singleton.h"

using namespace std;

class Logger : public Singleton<Logger>{
public:
	Logger();
	virtual ~Logger();

	stringstream message;

	void log(string type);
	void log(string type, string name);
	string bashColor(string message, string color);
	string composeColor(int background, int foreground);
};
