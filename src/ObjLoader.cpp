/*
 * ObjLoader.cpp
 *
 *  Created on: Jun 2, 2010
 *      Author: bmonkey
 */

#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <boost/foreach.hpp>

ObjLoader::ObjLoader() {

}

ObjLoader::~ObjLoader() {
	// TODO Auto-generated destructor stub
}

void ObjLoader::readFile(string file)
{
	cout << "Loading obj " << file << "\n";
	file = meshDir + file;
	ifstream fileStream(file.c_str());
	string str;
	vector<GLfloat> vertices, uvCoords, normals;
	vector<GLuint> indicies;
	GLubyte vertexCount;
	float x,y,z,w;
	int a,b,c,d,e,f,g,h,i,j,k,l;
	while (getline(fileStream, str)) {
		//cout << str << "\n";
		if (sscanf(str.c_str(), "v %f %f %f", &x, &y, &z)){
			cout << "Vertex: " << x<<" " << y << " " << z <<"\n";
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			//indicies.push_back(vertexCount);
			//vertexCount++;
		}

		/*
		 * Quads

		else if (sscanf(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l)){
			cout << "Index Quad 3: " << a<<" " << d << " " << g << " " << j << "\n";
			indicies.push_back(a);
			indicies.push_back(d);
			indicies.push_back(g);
			indicies.push_back(j);
		}else if (sscanf(str.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &a, &b, &c, &d, &e, &f, &g, &h)){
			cout << "Index Quad 2: " << a<<" " << b << " " << c << " " << d << "\n";
			indicies.push_back(a);
			indicies.push_back(c);
			indicies.push_back(e);
			indicies.push_back(g);
		}else if (sscanf(str.c_str(), "f %f %f %f %f", &x, &y, &z, &w)){
			cout << "Index Quad 1: " << x<<" " << y << " " << z << " " << w << "\n";
			indicies.push_back(x);
			indicies.push_back(y);
			indicies.push_back(z);
			indicies.push_back(w);
		}
		*/

		//Triangles

		else if (sscanf(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &b, &c, &d, &e, &f, &g, &h, &i)){
			cout << "Index Triangle 3: " << a<<" " << d << " " << g << " " << "\n";
			indicies.push_back(a);
			indicies.push_back(d);
			indicies.push_back(g);
		}else if (sscanf(str.c_str(), "f %d/%d %d/%d %d/%d", &a, &b, &c, &d, &e, &f)){
			cout << "Index Triangle 2: " << a<<" " << b << " " << c << " " << "\n";
			indicies.push_back(a);
			indicies.push_back(c);
			indicies.push_back(e);
		}else if (sscanf(str.c_str(), "f %f %f %f", &x, &y, &z)){
			cout << "Index Triangle 1: " << x<<" " << y << " " << z << "\n";
			indicies.push_back(x);
			indicies.push_back(y);
			indicies.push_back(z);
		}else if (sscanf(str.c_str(), "vt %f %f", &x, &y)){
			cout << "UV: " << x<<" " << y << "\n";
			uvCoords.push_back(x);
			uvCoords.push_back(y);
		}else if (sscanf(str.c_str(), "vn %f %f %f", &x, &y, &z)){
			cout << "Normals: " << x<<" " << y << " " << z << "\n";
			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}

	}

	//obj starts counting at 1
	cout << "Index:\t";
    BOOST_FOREACH( GLuint index, indicies )
    {
    	index--;
    	cout <<(int)index<<", ";
    }
    cout << "\n";

    mesh = new Mesh(vertices,vertices,normals,normals,normals,uvCoords,indicies);
}
