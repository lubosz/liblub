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

Mesh * ObjLoader::readFile(string file)
{

	string str;

	vector<GLfloat> vertices, uvCoords, normals,
					hardVertices, hardUvCoords, hardNormals;
	vector<GLuint> vertIndex, normalIndex, uvIndex,
					hardIndex;

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
		else if (sscanf(str.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vertIndex1, &uvIndex1, &normalIndex1,
				&vertIndex2, &uvIndex2, &normalIndex2,
				&vertIndex3, &uvIndex3, &normalIndex3)) {

			if (DEBUG) cout << "IndexVert:\t" << vertIndex1<< " " << vertIndex2 << " " << vertIndex3 << "\n";
			if (DEBUG) cout << "IndexNormal:\t" << normalIndex1<< " " << normalIndex2 << " " << normalIndex3 << "\n";
			if (DEBUG) cout << "IndexUV:\t" << uvIndex1 << " " << uvIndex2 << " " << uvIndex3 << "\n";
/*
			if (DEBUG) cout << "1:\t" << vertIndex1<< " " << normalIndex1 << " " << uvIndex1 << "\n";
			if (DEBUG) cout << "2:\t" << vertIndex2<< " " << normalIndex2 << " " << uvIndex2 << "\n";
			if (DEBUG) cout << "3:\t" << vertIndex3 << " " << normalIndex3 << " " << uvIndex3 << "\n";
	*/
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
	if (DEBUG) cout << "Vert Index:\t";
    BOOST_FOREACH( GLuint index, vertIndex )
    {
    	index--;
    	if (DEBUG) cout <<(int)index<<", ";
    }
    if (DEBUG) cout << "\n";
	if (DEBUG) cout << "Normal Index:\t";
    BOOST_FOREACH( GLuint index, normalIndex )
    {
    	index--;
    	if (DEBUG) cout <<(int)index<<", ";
    }
    if (DEBUG) cout << "\n";
	if (DEBUG) cout << "UV Index:\t";
    BOOST_FOREACH( GLuint index, uvIndex )
    {
    	index--;
    	if (DEBUG) cout <<(int)index<<", ";
    }
    if (DEBUG) cout << "\n";

    unsigned numFaces = vertIndex.size()/3;
    cout << "Points:" << points << " Indexsize:" << vertIndex.size() <<"\n";

    for (unsigned faceIndex = 0; faceIndex < numFaces; faceIndex++){
    //for (GLuint pointIndex = 0; pointIndex < vertIndex.size(); pointIndex++){
    	//for (unsigned indexIndex = 0; indexIndex < vertIndex.size(); indexIndex++){
    		//if ()

/*    		hardVertices.push_back(vertices[vertIndex[pointIndex]*3]);
    		hardVertices.push_back(vertices[vertIndex[pointIndex]*3+1]);
    		hardVertices.push_back(vertices[vertIndex[pointIndex]*3+2]);

    		hardNormals.push_back(vertices[normalIndex[pointIndex]*3]);
    		hardNormals.push_back(vertices[normalIndex[pointIndex]*3+1]);
    		hardNormals.push_back(vertices[normalIndex[pointIndex]*3+2]);

    		hardUvCoords.push_back(uvCoords[uvIndex[pointIndex]*2]);
    		hardUvCoords.push_back(uvCoords[uvIndex[pointIndex]*2+1]);*/

    	unsigned pointIndex = faceIndex * 3;
    	unsigned firstVert = vertIndex[pointIndex];



		hardNormals.push_back(normals[normalIndex[pointIndex]*3]);
		hardNormals.push_back(normals[normalIndex[pointIndex]*3+1]);
		hardNormals.push_back(normals[normalIndex[pointIndex]*3+2]);

		hardUvCoords.push_back(uvCoords[uvIndex[pointIndex]*2]);
		hardUvCoords.push_back(uvCoords[uvIndex[pointIndex]*2+1]);

    		//hardIndex.push_back(pointIndex);
    	//}

    }

    //cout << "Index Lengths:\t" << vertIndex.size() << " " << normalIndex.size() << " " << uvIndex.size() << "\n";
    cout << "Vertex Count:\t" << hardVertices.size() << "\tFaces:\t" << hardVertices.size()/3 << "\n";

    mesh = new Mesh();

    mesh->addBuffer(hardVertices,3,"in_Vertex");
    mesh->addBuffer(hardNormals,3,"in_Normal");
    mesh->addBuffer(hardUvCoords,2,"in_Uv");
    mesh->addElementBuffer(hardIndex);

    /*
    mesh->addBuffer(vertices,3,"in_Vertex");
    //mesh->addElementBuffer(vertIndex);
    mesh->addBuffer(normals,3,"in_Normal");
    //mesh->addElementBuffer(normalIndex);
    mesh->addBuffer(uvCoords,2,"in_Uv");
    mesh->addElementBuffer(uvIndex);
    */

    return mesh;
}

void ObjLoader::addFace(vector<GLfloat> * hardCopy, vector<GLfloat> * coords, unsigned firstCoord, unsigned length){
	for (unsigned i = 0; i < length; i++){
		hardCopy->push_back(coords[  firstCoord+i   ]);
	}
}

void ObjLoader::addPoint(vector<GLfloat> * hardCopy, vector<GLfloat> * coords, unsigned firstCoord, unsigned length){
	for (unsigned i = 0; i < length; i++){
		hardCopy->push_back(coords[  firstCoord+i   ]);
	}
}
