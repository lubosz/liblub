/*
 * Config.h
 *
 *  Created on: Dec 20, 2010
 *      Author: bmonkey
 */

#include "Singleton.h"
#include <string>

using namespace std;

class Config : public Singleton<Config> {
public:
	Config();
	virtual ~Config();

	string configPath;

	bool isEnabled(string value);
};

