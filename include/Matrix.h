/*
 * Matrix.h
 *
 *  Created on: Sep 10, 2010
 *      Author: bmonkey
 */


#pragma once
#include "common.h"
#include "ShaderProgram.h"
#include <vector>

class ShaderProgram;

#define EMPTY_MATRIX4  { 0.0, 0.0, 0.0, 0.0,\
                         0.0, 0.0, 0.0, 0.0,\
                         0.0, 0.0, 0.0, 0.0,\
                         0.0, 0.0, 0.0, 0.0 }

#define IDENTITY_MATRIX4 { 1.0, 0.0, 0.0, 0.0,\
                           0.0, 1.0, 0.0, 0.0,\
                           0.0, 0.0, 1.0, 0.0,\
                           0.0, 0.0, 0.0, 1.0 }


const GLfloat identitymatrix[16] = IDENTITY_MATRIX4;

typedef enum {
	X_AXIS,
	Y_AXIS,
	Z_AXIS
} AXIS;

class Matrix {
public:
	Matrix();
	virtual ~Matrix();
	/* An identity matrix we use to perform the equivalant of glLoadIdentity */
	void identity();
	void translate(vector<float> translation);
	void scale(float size);
	GLfloat & at(unsigned position);

	/* Rotate a matrix by an angle on a X, Y, or Z axis specified by the AXIS enum*/
	void rotate(GLfloat angle, AXIS axis);

	/* Perform translation operations on a matrix */
	void translate(GLfloat x, GLfloat y, GLfloat z);

	void bind(ShaderProgram * program, string name);

	void bindNormalMatrix(ShaderProgram * program);

	void multiply(Matrix * otherMatrix);

	GLfloat matrix[16]; /* Our model matrix  */
private:


	/* Multiply 4x4 matrix m1 by 4x4 matrix m2 and store the result in m1 */
	void multiply4x4(GLfloat *otherMatrix);
	void transpose3x3(GLfloat *matrix);

};
