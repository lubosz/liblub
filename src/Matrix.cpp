/*
 * Matrix.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "Matrix.h"

Matrix::Matrix(unsigned width, unsigned height) {
	this->height = height;
	this->width = width;
	elements = width * height;
	//data = new vector<vector<GLfloat> >();

	  // Create a 3D array that is 3 x 4 x 2
	  typedef boost::multi_array<GLfloat, 2> myMatrix;
	  typedef myMatrix::index index;
	  myMatrix A(boost::extents[width][height]);

		/* The four vericies of a tetrahedron */
		const GLfloat tetrahedron[4][3] = {
		{  1.0,  1.0,  1.0  },   /* index 0 */
		{ -1.0, -1.0,  1.0  },   /* index 1 */
		{ -1.0,  1.0, -1.0  },   /* index 2 */
		{  1.0, -1.0, -1.0  } }; /* index 3 */


	  // Assign values to the elements
	  int values = 0;
	  for(index i = 0; i != width; ++i)
	    for(index j = 0; j != height; ++j)
	        A[i][j] = values++;


}

Matrix::~Matrix() {
	// TODO Auto-generated destructor stub
}

void Matrix::readArray(GLfloat * myArray){
	GLfloat myAss[width][height];

	for (int w; w < width; w++){
		vector<GLfloat>  tempVector = vector<GLfloat>();
		for (int h; h < height; h++){
			tempVector.push_back(myAss[w][h]);
		}
		data.push_back(tempVector);
	}

}

void Matrix::writeArray(GLfloat * myArray){

}
