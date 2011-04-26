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
#include <cstring>
#include "common/Singleton.h"

#define ShortFile std::strrchr(__FILE__, '/')+1
#define glError Logger::Instance().checkGlError(ShortFile,__LINE__)

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
	void checkGlError(const char* file, int line);
	string bashColor(string message, string color);
	string composeColor(int background, int foreground);
};
