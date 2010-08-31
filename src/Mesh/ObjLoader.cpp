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

ObjLoader::ObjLoader(string file) {

	cout << "Loading obj " << file << "\n";
	filePath = meshDir + file;

	readFile();
	//obj starts counting at 1 opengl at 0
	if (DEBUG) cout << "Vert Index:\t";
	decrementIndex(vertIndex);
	if (DEBUG) cout << "Normal Index:\t";
	decrementIndex(normalIndex);
	if (DEBUG) cout << "UV Index:\t";
	decrementIndex(uvIndex);


	//reOrderHardCopy(vertices,normals,uvCoords,vertIndex,normalIndex,uvIndex);

if(DEBUG){
    cout 	<< "\nIndexed Vertex Count:\t"
    		<< "\nIndexSize/3 | Faces:\t" << vertIndex.size()/3
			<< "\nIndexSize | Points:\t" << vertIndex.size() << " | " << vertices.size()/3 << " " << normals.size()/3<< " " << uvCoords.size()/2
			<< "\nIndexSize*3 | Tripel Coords:\t" << vertIndex.size()*3 << " | " << vertices.size()<< " " << normals.size()
			<< "\nIndexSize*2 | Tupel Coords:\t" << vertIndex.size()*2 << " | " << uvCoords.size()
			<<"\n";

    cout << "Vert Index " << vertIndex.size() << "\n";
    cout << "Faces " << faces.size() << "\n";
    cout << "hardVertices Index" << hardVertices.size() << "\n";
    cout << "vertices Index" << vertices.size() << "\n";

}

    hardCopyIndex();


    //cout << "Index Lengths:\t" << vertIndex.size() << " " << normalIndex.size() << " " << uvIndex.size() << "\n";
    //cout << "Vertex Count:\t" << hardVertices.size() << "\tFaces:\t" << hardVertices.size()/3 << "\n";

    //vector<GLfloat> glVertices, glUvCoords, glNormals;
    mesh = createMesh();
}

ObjLoader::~ObjLoader() {
	// TODO Auto-generated destructor stub
}

void ObjLoader::readFile()
{

	unsigned vertIndex1, vertIndex2, vertIndex3, vertIndex4,
		normalIndex1, normalIndex2, normalIndex3, normalIndex4,
		uvIndex1, uvIndex2, uvIndex3, uvIndex4;

	float vertX, vertY, vertZ,
		normalX, normalY, normalZ,
		uvX, uvY;


	ifstream fileStream(filePath.c_str());

	string str;

	while (getline(fileStream, str)) {
		//Vertex
		if (sscanf(str.c_str(), "v %f %f %f", &vertX, &vertY, &vertZ)){
			if (DEBUG) cout << "Vertex: " << vertX<<" " << vertY << " " << vertZ <<"\n";
			vertices.push_back(Vector3D(vertX, vertY, vertZ));
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
/*
			if (DEBUG) cout << "IndexVert:\t" << vertIndex1<< " " << vertIndex2 << " " << vertIndex3 << "\n";
			if (DEBUG) cout << "IndexNormal:\t" << normalIndex1<< " " << normalIndex2 << " " << normalIndex3 << "\n";
			if (DEBUG) cout << "IndexUV:\t" << uvIndex1 << " " << uvIndex2 << " " << uvIndex3 << "\n";
*/
/*
			if (DEBUG) cout << "1:\t" << vertIndex1<< " " << normalIndex1 << " " << uvIndex1 << "\n";
			if (DEBUG) cout << "2:\t" << vertIndex2<< " " << normalIndex2 << " " << uvIndex2 << "\n";
			if (DEBUG) cout << "3:\t" << vertIndex3 << " " << normalIndex3 << " " << uvIndex3 << "\n";
	*/

			Face face = Face(vertIndex1-1,vertIndex2-1,vertIndex3-1);
			face.setNormalIndices(normalIndex1-1,normalIndex2-1,normalIndex3-1);
			face.setUvIndices(uvIndex1-1,uvIndex2-1,uvIndex3-1);
			faces.push_back(face);
			/*
			vertIndex.push_back(vertIndex1);
			vertIndex.push_back(vertIndex2);
			vertIndex.push_back(vertIndex3);

			normalIndex.push_back(normalIndex1);
			normalIndex.push_back(normalIndex2);
			normalIndex.push_back(normalIndex3);

			uvIndex.push_back(uvIndex1);
			uvIndex.push_back(uvIndex2);
			uvIndex.push_back(uvIndex3);
			*/

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
		}else if (sscanf(str.c_str(), "f %d %d %d", &vertIndex1, &vertIndex2, &vertIndex3)){
			if (DEBUG) cout << "Index Triangle 1: " << vertIndex1<<" " << vertIndex2 << " " << vertIndex3 << "\n";
			vertIndex.push_back(vertIndex1);
			vertIndex.push_back(vertIndex2);
			vertIndex.push_back(vertIndex3);

		//UV
		}else if (sscanf(str.c_str(), "vt %f %f", &uvX, &uvY)){
			if (DEBUG) cout << "UV: " << uvX<<" " << uvY << "\n";
			uvCoords.push_back(Vector2D(uvX,uvY));

		//Normal
		}else if (sscanf(str.c_str(), "vn %f %f %f", &normalX, &normalY, &normalZ)){
			if (DEBUG) cout << "Normals: " << normalX<<" " << normalY << " " << normalZ << "\n";
			normals.push_back(Vector3D(normalX, normalY, normalZ));
		}

	}


}

void ObjLoader::hardCopyIndex(){
	hardIndexSize = 0;
    BOOST_FOREACH( Face face, faces )
    {
    	addTriangle(face);
    }
}


Mesh * ObjLoader::createMesh(){
    Mesh * mesh = new Mesh();

    mesh->addBuffer(hardVertices,3,"in_Vertex");
    mesh->addBuffer(hardNormals,3,"in_Normal");
    mesh->addBuffer(hardUvCoords,2,"in_Uv");
    mesh->addElementBuffer(hardIndex);

    /*
     * One Element Buffer per Buffer?
    mesh->addBuffer(vertices,3,"in_Vertex");
    //mesh->addElementBuffer(vertIndex);
    mesh->addBuffer(normals,3,"in_Normal");
    //mesh->addElementBuffer(normalIndex);
    mesh->addBuffer(uvCoords,2,"in_Uv");
    mesh->addElementBuffer(uvIndex);
    */

    return mesh;
}

void ObjLoader::decrementIndex(vector<GLuint> &indices){
    BOOST_FOREACH( GLuint index, indices )
    {
    	index--;
    	if (DEBUG) cout <<(int)index<<", ";
    }
    if (DEBUG) cout << "\n";
}

/*
void ObjLoader::reOrderHardCopy(vector<GLfloat> &vertices,vector<GLfloat> &normals,vector<GLfloat> &uvCoords,vector<GLuint> &vertIndex,vector<GLuint> &normalIndex,vector<GLuint> &uvIndex){


    unsigned numFaces = vertIndex.size()/3;
    //cout << "Vertex Count:\t" << hardVertices.size()<< "NumFaces:" << numFaces << " IndexSize:" << vertIndex.size() <<"\n";

    for (unsigned faceIndex = 0; faceIndex < numFaces; faceIndex++){
    	if (DEBUG) cout << "Vert\n";
    	addTriangle(faceIndex,&hardVertices,&vertices,&vertIndex, 3);
    	if (DEBUG) cout << "Normal\n";
    	addTriangle(faceIndex,&hardNormals,&normals,&normalIndex, 3);
    	if (DEBUG) cout << "UV\n";
    	addTriangle(faceIndex,&hardUvCoords,&uvCoords,&uvIndex, 2);
    }

    for (unsigned i = 0; i < vertIndex.size(); i++){
    	hardIndex.push_back(i);
    }

    if (DEBUG){
		cout << "Hard Verts\n";
		printVector(&hardVertices, 3);
		cout << "Hard Normals\n";
		printVector(&hardNormals, 3);
		cout << "Hard UV\n";
		printVector(&hardUvCoords, 2);
		cout << "Hard Index\n";
		//printVector(&hardIndex, 10);
		BOOST_FOREACH( GLuint index, hardIndex )
		{
			cout <<(int)index<<", ";
		}
    }

    vertices = hardVertices;
    normals = hardNormals;
    uvCoords = hardUvCoords;
    vertIndex = hardIndex;
}
*/

void ObjLoader::addTriangle(Face face){
	//cout << "Add Triangle\n";
	for (int i = 0; i < 3; i++){
		Vector3D vertex = vertices[face.vertIndices[i]];
		hardVertices.push_back(vertex.x);
		hardVertices.push_back(vertex.y);
		hardVertices.push_back(vertex.z);
	}
	hardIndex.push_back(hardIndexSize);
	hardIndexSize++;

	for (int i = 0; i < 3; i++){
		Vector3D normal = normals[face.normalIndices[i]];
		hardNormals.push_back(normal.x);
		hardNormals.push_back(normal.y);
		hardNormals.push_back(normal.z);
	}
	hardIndex.push_back(hardIndexSize);
	hardIndexSize++;

	for (int i = 0; i < 3; i++){
		Vector2D uv = uvCoords[face.uvIndices[i]];
		hardUvCoords.push_back(uv.x);
		hardUvCoords.push_back(uv.y);
	}
	hardIndex.push_back(hardIndexSize);
	hardIndexSize++;
	/*
	unsigned pointIndex = faceIndex * 3;
	if (DEBUG) cout << "Face: " << faceIndex <<"\n";
	addPoint(hardCopy, coords, unsigned(index->at(pointIndex)), length);
	addPoint(hardCopy, coords, unsigned(index->at(pointIndex+1)), length);
	addPoint(hardCopy, coords, unsigned(index->at(pointIndex+2)), length);
	*/
}

void ObjLoader::addPoint(vector<GLfloat> * hardCopy, vector<GLfloat> * coords, unsigned firstCoord, unsigned length){
	if (DEBUG) cout << "Point: ";
	for (unsigned i = 0; i < length; i++){
		if (DEBUG) cout << coords->at(firstCoord+i) <<", ";

		hardCopy->push_back(coords->at(firstCoord+i));
	}
	if (DEBUG) cout << "\n";
}

void ObjLoader::printVector(vector<GLfloat> * printMe, unsigned length){
    for (unsigned i = 0; i < printMe->size(); i++ )
    {
    	 if((i)%length == 0) cout << i/length << ": ";
         cout << printMe->at(i)<<", ";
    	 if((i+1)%length == 0) cout << "\n";
    }
}

Mesh * ObjLoader::getMesh() const
{
    return mesh;
}
