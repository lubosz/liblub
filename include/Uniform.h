/*
 * Uniform.h
 *
 *  Created on: Dec 16, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <vector>

using namespace std;

class Uniform {
public:
	Uniform(string name, const vector<float> & values);
	virtual ~Uniform();

	string name;
	vector<float> values;
};

class Uniformi {
public:
	Uniformi(string name, const vector<int> & values);
	virtual ~Uniformi();

	string name;
	vector<int> values;
};
