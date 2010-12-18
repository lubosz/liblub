/*
 * Uniform.cpp
 *
 *  Created on: Dec 16, 2010
 *      Author: bmonkey
 */

#include "Uniform.h"

Uniform::Uniform(string name, const vector<float> & values) : name(name) ,values(values){
}

Uniform::~Uniform() {
	// TODO Auto-generated destructor stub
}

Uniformi::Uniformi(string name, const vector<int> & values) : name(name) ,values(values){
}

Uniformi::~Uniformi() {
	// TODO Auto-generated destructor stub
}
