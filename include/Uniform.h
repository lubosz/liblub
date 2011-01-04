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

template<class T>
class Uniform {
public:
	Uniform(string name, const vector<T> & values): name(name) ,values(values){}
	virtual ~Uniform() {};

	string name;
	vector<T> values;
};
