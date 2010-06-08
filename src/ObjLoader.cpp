/*
 * ObjLoader.cpp
 *
 *  Created on: Jun 2, 2010
 *      Author: bmonkey
 */

#include "common.h"
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

	string str;

	vector<GLfloat> vertices, uvCoords, normals, glUv, glNormal;
	vector<GLuint> vertIndex, normalIndex, uvIndex, glIndex;

	int vertIndex1, vertIndex2, vertIndex3, vertIndex4,
		normalIndex1, normalIndex2, normalIndex3, normalIndex4,
		uvIndex1, uvIndex2, uvIndex3, uvIndex4;

	float vert1, vert2, vert3,
		normal1, normal2, normal3,
		uv1, uv2;

	cout << "Loading obj " << file << "\n";
	file = meshDir + file;
	ifstream fileStream(file.c_str());

	while (getline(fileStream, str)) {
		//Vertex
		if (sscanf(str.c_str(), "v %f %f %f", &vert1, &vert2, &vert3)){
			if (DEBUG) cout << "Vertex: " << vert1<<" " << vert2 << " " << vert3 <<"\n";
			vertices.push_back(vert1);
			vertices.push_back(vert2);
			vertices.push_back(vert3);
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

		//Triangle Indices

		//Index: Vertex Normal UV
		else if (sscanf(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertIndex1, &normalIndex1, &uvIndex1, &vertIndex2, &normalIndex2, &uvIndex2, &vertIndex3, &normalIndex3, &uvIndex3)){
			if (DEBUG) cout << "Index Triangle 3: " << vertIndex1<<" " << vertIndex2 << " " << vertIndex3 << " " << "\n";
			vertIndex.push_back(vertIndex1);
			vertIndex.push_back(vertIndex2);
			vertIndex.push_back(vertIndex3);

			normalIndex.push_back(normalIndex1);
			normalIndex.push_back(normalIndex2);
			normalIndex.push_back(normalIndex3);

			uvIndex.push_back(uvIndex1);
			uvIndex.push_back(uvIndex2);
			uvIndex.push_back(uvIndex3);

		//Index: Vertex Normal
		}else if (sscanf(str.c_str(), "f %d/%d %d/%d %d/%d", &vertIndex1, &normalIndex1, &vertIndex2, &normalIndex2, &vertIndex3, &normalIndex3)){
			if (DEBUG) cout << "Index Triangle 2: " << &vertIndex1<<" " << &vertIndex2 << " " << &vertIndex3 << " " << "\n";
			vertIndex.push_back(vertIndex1);
			vertIndex.push_back(vertIndex2);
			vertIndex.push_back(vertIndex3);

			normalIndex.push_back(normalIndex1);
			normalIndex.push_back(normalIndex2);
			normalIndex.push_back(normalIndex3);

		//Index: Vertex
		}else if (sscanf(str.c_str(), "f %f %f %f", &vertIndex1, &vertIndex2, &vertIndex3)){
			if (DEBUG) cout << "Index Triangle 1: " << vertIndex1<<" " << vertIndex2 << " " << vertIndex3 << "\n";
			vertIndex.push_back(vertIndex1);
			vertIndex.push_back(vertIndex2);
			vertIndex.push_back(vertIndex3);

		//UV
		}else if (sscanf(str.c_str(), "vt %f %f", &uv1, &uv2)){
			if (DEBUG) cout << "UV: " << uv1<<" " << uv2 << "\n";
			uvCoords.push_back(uv1);
			uvCoords.push_back(uv2);

		//Normal
		}else if (sscanf(str.c_str(), "vn %f %f %f", &normal1, &normal2, &normal3)){
			if (DEBUG) cout << "Normals: " << normal1<<" " << normal2 << " " << normal3 << "\n";
			normals.push_back(normal1);
			normals.push_back(normal2);
			normals.push_back(normal3);
		}

	}

	//obj starts counting at 1
	if (DEBUG) cout << "Index:\t";
    BOOST_FOREACH( GLuint index, vertIndex )
    {
    	index--;
    	if (DEBUG) cout <<(int)index<<", ";
    }
    if (DEBUG) cout << "\n";

    //cout << "Index Lengths:\t" << vertIndex.size() << " " << normalIndex.size() << " " << uvIndex.size() << "\n";
    cout << "Vertex Count:\t" << vertices.size() << "\n";

    mesh = new Mesh(vertices,vertices,normals,normals,normals,uvCoords,vertIndex);
}
