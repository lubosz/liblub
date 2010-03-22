#pragma once

#ifndef GLfloat
#include <GL3/gl3.h>
#endif

#include <string>

using namespace std;

#define EMPTY_MATRIX4  { 0.0, 0.0, 0.0, 0.0,\
                         0.0, 0.0, 0.0, 0.0,\
                         0.0, 0.0, 0.0, 0.0,\
                         0.0, 0.0, 0.0, 0.0 }

#define IDENTITY_MATRIX4 { 1.0, 0.0, 0.0, 0.0,\
                           0.0, 1.0, 0.0, 0.0,\
                           0.0, 0.0, 1.0, 0.0,\
                           0.0, 0.0, 0.0, 1.0 }

typedef enum {
X_AXIS,
Y_AXIS,
Z_AXIS
} AXIS;

/* Multiply 4x4 matrix m1 by 4x4 matrix m2 and store the result in m1 */
void multiply4x4(GLfloat *m1, GLfloat *m2);

/* Generate a perspective view matrix using a field of view angle fov,
 * window aspect ratio, near and far clipping planes */
void perspective(GLfloat *matrix, GLfloat fov, GLfloat aspect, GLfloat nearz, GLfloat farz);

/* Perform translation operations on a matrix */
void translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z);

/* Rotate a matrix by an angle on a X, Y, or Z axis specified by the AXIS enum*/
void rotate(GLfloat *matrix, GLfloat angle, AXIS axis);

/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(string file);

