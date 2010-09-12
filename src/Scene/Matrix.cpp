/*
 * Matrix.cpp
 *
 *  Created on: Sep 10, 2010
 *      Author: bmonkey
 */

#include "Matrix.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

Matrix::Matrix() {
	memset(matrix, 0, sizeof(GLfloat) * 16);
}

Matrix::~Matrix() {
	// TODO Auto-generated destructor stub
}

GLfloat & Matrix::at(unsigned position){
	return matrix[position];
}
/* Rotate a matrix by an angle on a X, Y, or Z axis */
void Matrix::rotate(GLfloat angle, AXIS axis)
{
    const GLfloat d2r = 0.0174532925199; /* PI / 180 */
    const int cos1[3] = { 5, 0, 0 };
    const int cos2[3] = { 10, 10, 5 };
    const int sin1[3] = { 6, 2, 1 };
    const int sin2[3] = { 9, 8, 4 };
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

    newmatrix[cos1[axis]] = cos(d2r * angle);
    newmatrix[sin1[axis]] = -sin(d2r * angle);
    newmatrix[sin2[axis]] = -newmatrix[sin1[axis]];
    newmatrix[cos2[axis]] = newmatrix[cos1[axis]];

    multiply4x4(newmatrix);
}

void Matrix::transpose3x3(GLfloat *matrix)
{
   GLfloat temp;
   for(int i=0;i<3;i++)
       for(int j=i+1;j<3;j++)
      {
         temp=matrix[i + (j*3)];
         matrix[i + (j*3)]=matrix[j + (i*3)];
         matrix[j + (i*3)]=temp;
      }
}

void Matrix::identity(){
    memcpy(matrix, identitymatrix, sizeof(GLfloat) * 16);
}

/* Perform scale operations on a matrix */
void Matrix::scale(GLfloat size)
{
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

/*
    newmatrix[0] = size;
    newmatrix[5] = size;
    newmatrix[10] = size;
*/
    newmatrix[15] = 1/size;
    //cout << "size:" << size << "\n";
    multiply4x4(newmatrix);
}

/* Perform translation operations on a matrix */
void Matrix::translate(GLfloat x, GLfloat y, GLfloat z)
{
    GLfloat newmatrix[16] = IDENTITY_MATRIX4;

    newmatrix[12] = x;
    newmatrix[13] = y;
    newmatrix[14] = z;

    multiply4x4(newmatrix);
}

void Matrix::translate(vector<float> translation){
    translate(translation.at(0), translation.at(1), translation.at(2));
}

/* Multiply 4x4 matrix m1 by 4x4 matrix m2 and store the result in m1 */
void Matrix::multiply4x4(GLfloat *otherMatrix)
{
    GLfloat temp[16];

    int x,y;

    for (x=0; x < 4; x++)
    {
        for(y=0; y < 4; y++)
        {
            temp[y + (x*4)] = (matrix[x*4] * otherMatrix[y]) +
                              (matrix[(x*4)+1] * otherMatrix[y+4]) +
                              (matrix[(x*4)+2] * otherMatrix[y+8]) +
                              (matrix[(x*4)+3] * otherMatrix[y+12]);
        }
    }

    memcpy(matrix, temp, sizeof(GLfloat) * 16);
}

void Matrix::multiply(Matrix * otherMatrix){
	multiply4x4(otherMatrix->matrix);
}

void Matrix::bind(ShaderProgram * program, string name){
    /* Bind our matrix variable to be a uniform called name in our shaderprogram */
	glUniformMatrix4fv(glGetUniformLocation(program->getReference(), name.c_str()), 1, GL_FALSE, matrix);
}

void Matrix::bindNormalMatrix(ShaderProgram * program){
    GLfloat normalMatrix[9] = {
                    matrix[0], matrix[1], matrix[2],
                    matrix[4], matrix[5], matrix[6],
                    matrix[8], matrix[9], matrix[10]
    };
	glUniformMatrix3fv(glGetUniformLocation(program->getReference(), "NormalMatrix"), 1, GL_FALSE, normalMatrix);
}
