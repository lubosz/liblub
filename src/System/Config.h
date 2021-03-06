/*
 * Copyright © 2010 Lubosz Sarnecki
 * Config.h
 *
 *  Created on: Dec 20, 2010
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "Common/Singleton.h"
#include "XmlReader.h"
#include <string>
#include <vector>
#include <QDomElement>
#include <QFile>

using std::string;
using std::vector;

template <typename T>
class ConfigOption {
 public:
    ConfigOption(const string& name, const vector <T> &optionVec)
	:name(name), optionVec(optionVec) {}
    ConfigOption(const string& name, const T &option)
    :name(name) {
        optionVec.push_back(option);
    }
	virtual ~ConfigOption() {}
	string name;
	vector <T> optionVec;
};

class Config : public Singleton<Config>, public XmlReader {
 public:
	Config();
	virtual ~Config();
    void addString(const string &name, const string &option);
	void appendOption(const QDomElement & optionNode);
    template<typename T> vector<T> getValues(const string & name, const vector<ConfigOption<T>* > & config);
    template<typename T> void setValues(const string & name, const vector<T> & values);
	template<typename T> T value(const string & name);
	template<typename T> vector<T> values(const string & name);
//	template<typename T> vector<T> splitValues(QString values);

    vector<ConfigOption<bool>* > bools;
    vector<ConfigOption<int>* > ints;
    vector<ConfigOption<string>* > strings;
    vector<ConfigOption<float>* > floats;

    QStringList getGLVersion();
    void createConfigFile(QFile *file);
    string getMediaPrefix();

	bool isEnabled(string value);
};

#endif
