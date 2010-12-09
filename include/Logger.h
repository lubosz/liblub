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

enum LogType { LOG_ERROR, LOG_WARNING, LOG_MESSAGE, LOG_DEBUG};


class Logger : public Singleton<Logger>{
public:
	Logger();
	virtual ~Logger();

	stringstream message;

	void log(LogType type);
	void log(LogType type, string name);
	string bashColor(string message, string color);
	string composeColor(int background, int foreground);
};
