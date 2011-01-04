/*
 * Config.h
 *
 *  Created on: Dec 20, 2010
 *      Author: bmonkey
 */

#include "Singleton.h"
#include <string>
#include <vector>
#include <QDomElement>

using namespace std;

template <typename T>
class ConfigOption{
public:
	ConfigOption(string name, vector <T> &optionVec):name(name),optionVec(optionVec) {}
	virtual ~ConfigOption() {}
	string name;
	vector <T> optionVec;
};

class Config : public Singleton<Config> {
public:
	Config();
	virtual ~Config();
	void load(const QString & fileName);
	void appendOption(const QDomElement & optionNode);
	string fileName;

	vector<ConfigOption<bool>> bools;
	vector<ConfigOption<int>> ints;
	vector<ConfigOption<string>> strings;
	vector<ConfigOption<float>> floats;


	bool isEnabled(string value);
};

