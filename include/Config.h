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
#include "XmlReader.h"

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

	template<typename T> T pushValue( QString& value );
	template<typename T>  vector<T> splitValues(QString values);

	vector<ConfigOption<bool>> bools;
	vector<ConfigOption<int>> ints;
	vector<ConfigOption<string>> strings;
	vector<ConfigOption<float>> floats;

	template<typename T> vector<T> getValue(string name);


	bool isEnabled(string value);
};

