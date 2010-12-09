/*
 * Logger.h
 *
 *  Created on: Dec 9, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include "Singleton.h"

using namespace std;

enum LogType { ERROR, WARNING, MESSAGE, DEBUG};


class Logger : public Singleton<Logger>{
public:
	Logger();
	virtual ~Logger();

	void log(string name, string message ,LogType type);
	string bashColor(string message, string color);
	string composeColor(int background, int foreground);
};
