/*
 * Copyright © 2010 Lubosz Sarnecki
 * Config.h
 *
 *  Created on: Dec 20, 2010
 */

#include "common/Singleton.h"
#include "XmlReader.h"
#include <string>
#include <vector>
#include <QDomElement>

using std::string;
using std::vector;

template <typename T>
class ConfigOption {
 public:
	ConfigOption(string name, vector <T> &optionVec)
	:name(name), optionVec(optionVec) {}
	virtual ~ConfigOption() {}
	string name;
	vector <T> optionVec;
};

class Config : public Singleton<Config>, public XmlReader {
 public:
	Config();
	virtual ~Config();
	void load(const QString & fileName);
	void appendOption(const QDomElement & optionNode);
	string fileName;
	template<typename T> vector<T> getValues(string name, const vector<ConfigOption<T>> & config);
	template<typename T> T value(const string & name);
	template<typename T> vector<T> values(const string & name);
	template<typename T> vector<T> splitValues(QString values);

	vector<ConfigOption<bool>> bools;
	vector<ConfigOption<int>> ints;
	vector<ConfigOption<string>> strings;
	vector<ConfigOption<float>> floats;


	bool isEnabled(string value);
};

