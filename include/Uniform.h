/*
 * Uniform.h
 *
 *  Created on: Dec 16, 2010
 *      Author: bmonkey
 */

#pragma once

#include <string>
#include <vector>
#include "common/OpenGL.h"

using namespace std;

template<class T>
class Uniform {
public:
	Uniform(string name, const vector<T> & values): name(name) ,values(values){}
	virtual ~Uniform() {};

	void init(GLuint program);
	string name;
	vector<T> values;
};
