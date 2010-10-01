/*
 * Geomerty.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: bmonkey
 */

#include "Geometry.h"
#include <iostream>
#include <QVector3D>
#include <QMatrix4x4>
#include <math.h>

using namespace std;

Mesh * Geometry::makePlane(){
	/*
	vector<GLfloat> vertices = {
		-1.0,  -1.0,  0.0,
		1.0, -1.0,  0.0,
		-1.0,  1.0, 0.0,
		1.0, 1.0, 0.0
	};
	*/
	/*
	vector<GLfloat> vertices = {
			-1.0,  -1.0,  -2.0,
			1.0, -1.0,  -2.0,
			-1.0,  1.0, -2.0,
			1.0, 1.0, -2.0
		};
	*/

	vector<GLfloat> vertices = {
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0, 1.0, -1.0,
			-1.0, 1.0, -1.0
		};

	vector<GLfloat> uvCoords = {
			0.0,  0.0,

		1.0,  0.0,
		1.0,  1.0,

		0.0,  1.0,


			/*
			1.0,  1.0,
			1.0,  1.2,
			1.2,  1.0,
			1.2,  1.2
	*/
	};

	vector<GLfloat> normals = {
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, 0.0, 1.0
	};

	//vector<GLuint> indicies = { 0, 1, 3, 3, 2, 0 };
	vector<GLuint> indicies = { 0, 1, 3, 3, 1, 2 };

	Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    //mesh->addBuffer(vertices,3,"in_Color");
    mesh->addBuffer(normals,3,"in_Normal");
    mesh->addBuffer(normals,3,"in_Tangent");
    mesh->addBuffer(uvCoords,2,"in_Uv");
    mesh->addElementBuffer(indicies);
    mesh->setDrawType(GL_TRIANGLES);
	return mesh;

}

Mesh * Geometry::makeTetrahedron(){
	vector<GLfloat> vertices = {
		1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0, -1.0,
		1.0, -1.0, -1.0
	};

	vector<GLfloat> vertexColors = {
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0
	};

	vector<GLfloat> uvCoords = {
		1.0,  0.0,
		0.0,  1.0,
		0.0,  0.0,
		1.0,  1.0
	};

	vector<GLfloat> normals = {
		1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0, -1.0,
		1.0, -1.0, -1.0
	};

	vector<GLuint> indicies = { 0, 1, 2, 3, 0, 1 };

	cout << "Making Tetrahedron!!";

	Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    mesh->addBuffer(vertexColors,3,"in_Color");
    mesh->addBuffer(normals,3,"in_Normal");
    mesh->addBuffer(uvCoords,2,"in_Uv");

    mesh->addElementBuffer(indicies);
    //mesh->setDrawType(GL_TRIANGLE_STRIP);
	return mesh;

}

Mesh * Geometry::makeCube(){

	vector<GLfloat> vertices = {
			/*
			1.0, 1.0, 1.0,
			1.0, -1.0, 1.0,
			-1.0, -1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, -1.0, -1.0,
			-1.0, -1.0, -1.0
			*/

			1.0, -1.0, -1.0,
			1.0, -1.0, 1.0,
			-1.0, -1.0, 1.0,
			-1.0, -1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0
	};

	vector<GLfloat> normals = {
/*
			0.0, 0.0, -1.0,
			-1.0, -0.0, -0.0,
			-0.0, -0.0, 1.0,
			-0.1, 0.0, 1.0,
			1.0, -0.0, 0.0,
			1.0, 0.0, 0.1,
			0.0, 1.0, -0.0,
			-0.0, -1.0, 0.0
*/

			/*
			-0.0, -1.0, 0.0,
			-0.0, -0.0, 1.0,
			-1.0, -0.0, -0.0,
			0.0, 0.0, -1.0,
			0.0, 0.0, -1.0,
			0.0, 1.0, -0.0,
			-1.0, -0.0, -0.0,
			0.0, 1.0, -0.0
			*/


	};


	vector<GLfloat> vertexColors = {
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0,
		1.0,  0.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  0.0,  1.0,
		1.0,  1.0,  1.0
	};

	vector<GLfloat> uvCoords = {
			/*
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0,
			*/
			1.0, 0.0,
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0,
			1.0, 1.0,
			0.0, 1.0

			/*
		1.0,  0.0,
		0.0,  1.0,
		0.0,  0.0,
		1.0,  1.0,
		1.0,  0.0,
		0.0,  1.0,
		0.0,  0.0,
		1.0,  1.0
		*/
	};

/*
	f 5/1/1 1/2/1 4/3/1
	f 5/1/1 4/3/1 8/4/1

	f 3/1/2 7/2/2 8/3/2
	f 3/1/2 8/3/2 4/4/2

	f 2/1/3 6/2/3 3/4/3
	f 6/2/4 7/3/4 3/4/4

	f 1/1/5 5/2/5 2/4/5
	f 5/2/6 6/3/6 2/4/6

	f 5/1/7 8/2/7 6/4/7
	f 8/2/7 7/3/7 6/4/7

	f 1/1/8 2/2/8 3/3/8
	f 1/1/8 3/3/8 4/4/8
*/

	//vector<GLuint> indicies = { 0, 1, 2, 3, 4, 5, 6, 7 };
	vector<GLuint> indicies = {
									4, 0, 3,
									4, 3, 7,

									1,5,2,
									5,6,2,


									2, 6, 7,
									2, 7, 3,

									0,4,1,
									4,5,1,

									4,7,5,
									7,6,5,

									0,1,2,
									0,2,3



	};

	Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    mesh->addBuffer(vertices,3,"in_Color");
    mesh->addBuffer(uvCoords,2,"in_Uv");
    //mesh->addBuffer(normals,3,"in_Normal");
    mesh->addBuffer(vertices,3,"in_Normal");
    mesh->addElementBuffer(indicies);
	return mesh;

}

float Geometry::randomize(float density, float randomness){
	//return density+(density/(rand()%variation))*randomness;

	float randomValue = 1/float((rand()%20)+1);
	/*
	cout << density <<"\n";
	cout << density+(density*randomValue*randomness) <<"\n\n";
	*/
	return density+(density*randomValue*randomness);
}


Mesh * Geometry::makeStars(vector<float> resolution, float density, float randomness){

	vector<GLfloat> vertices, colors;
	vector<GLuint> indicies;

	unsigned i = 0;

	srand ( time(NULL) );
	//(density+(density/rand()%variation)*randomness)

	for (float x = 0; x < resolution.at(0); x+=randomize(density, randomness)){
		for (float y = 0; y < resolution.at(1); y+=randomize(density, randomness)){
			for (float z = 0; z < resolution.at(2); z+=randomize(density, randomness)){
				/*
				cout << "Point: x" << x <<" y " << y << " z " << z << "\n";
				cout << "Density: " << density<< "\n"
				<< resolution.at(0)<< "\n"
				<< resolution.at(1)<< "\n"
				<< resolution.at(2)<< "\n";

				float blubb = rand()%100;
				cout << "rand: " << blubb << "\n";
							*/
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				colors.push_back(float(int(x)%255)/256);
				colors.push_back(float(int(y)%255)/256);
				colors.push_back(float(int(z)%255)/256);

				indicies.push_back(i);

				i++;
			}
		}

	}
	cout << i << " Generated Stars\n";

	Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    mesh->addBuffer(colors,3,"in_Color");
    //mesh->addBuffer(uvCoords,2,"in_Uv");
    //mesh->addBuffer(normals,3,"in_Normal");
    //mesh->addBuffer(vertices,3,"in_Normal");
    mesh->addElementBuffer(indicies);
	return mesh;

}

Mesh * Geometry::makeSpiral(int resolution, float density, float randomness){

	vector<GLfloat> vertices, colors;
	vector<GLuint> indicies;

	QVector3D point = QVector3D(1,0,0);
	QMatrix4x4 rotation = QMatrix4x4();
	rotation.rotate(0.5, 0.0, 1.0, 0.0);

	QMatrix4x4 rotation2 = QMatrix4x4();
	rotation2.rotate(0.5, 0.0, 0.0, 1.0);


	unsigned i = 1;

	srand ( time(NULL) );
	for(i; i < resolution; i++){
		QVector3D tempPoint = point.normalized();
		//point += (tempPoint * pow ( i, 1.0 / 15.0 )/10.0);
		point += (tempPoint/20.0);

		point = rotation * point;
/*
		cout << point.x() << " "<< point.y() << " " << point.z() << "\n";
		vertices.push_back(point.x());
		vertices.push_back(point.y());
		vertices.push_back(point.z());

		colors.push_back(255.0);
		colors.push_back(255.0);
		colors.push_back(255.0);

		indicies.push_back(i);
*/

		QVector3D newPoint = point;
		for(int j = 0; j < 100; j++){
			newPoint = rotation2 * newPoint;

			QVector3D tempPoint2 = newPoint.normalized();
			//point += (tempPoint * pow ( i, 1.0 / 15.0 )/10.0);
			newPoint += (tempPoint2/20.0);



			//cout << point.x() << " "<< point.y() << " " << point.z() << "\n";
			vertices.push_back(newPoint.x());
			vertices.push_back(newPoint.y());
			vertices.push_back(newPoint.z());

			colors.push_back(1.0-(float(int(newPoint.x())%255)/256));
			colors.push_back(1.0-(float(int(newPoint.y())%255)/256));
			colors.push_back(1.0-(float(int(newPoint.z())%255)/256));

			indicies.push_back(i);
			i++;

		}


	}

	//(density+(density/rand()%variation)*randomness)

/*
	for (float x = 0; x < resolution; x++){
		for (float y = 0; y < resolution; y++){
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(0);

				colors.push_back(float(int(x)%255)/256);
				colors.push_back(float(int(y)%255)/256);
				colors.push_back(float(int(y)%255)/256);

				indicies.push_back(i);

				i++;
			}
		}
*/

	cout << i << " Generated Stars\n";

	Mesh * mesh = new Mesh();
    mesh->addBuffer(vertices,3,"in_Vertex");
    mesh->addBuffer(colors,3,"in_Color");
    //mesh->addBuffer(uvCoords,2,"in_Uv");
    //mesh->addBuffer(normals,3,"in_Normal");
    //mesh->addBuffer(vertices,3,"in_Normal");
    mesh->addElementBuffer(indicies);
	return mesh;

}
