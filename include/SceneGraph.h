#pragma once

#include "common.h"
#include "ShaderProgram.h"
#include "Singleton.h"
#include "Node.h"
#include "Material.h"
#include <stdio.h>
#include <string.h>

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

class SceneGraph: public Singleton<SceneGraph>
{
public:
	friend class Singleton<SceneGraph>;

	vector<Node> sceneNodes;
	vector<Material*> materials;


	/* Multiply 4x4 matrix m1 by 4x4 matrix m2 and store the result in m1 */
	void multiply4x4(GLfloat *m1, GLfloat *m2);


	/* Perform translation operations on a matrix */
	void translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z);

	/* Rotate a matrix by an angle on a X, Y, or Z axis specified by the AXIS enum*/
	void rotate(GLfloat *matrix, GLfloat angle, AXIS axis);

	void transpose3x3(GLfloat *matrix);

	void animate(float frameCount);
	void transform(float frameCount);
	void transform();
	void bindShaders(ShaderProgram * shaderProgram);
	void translate(float x, float y, float z);
	void translate(vector<float> translation);
	void addNode(string name, vector<float> position, Mesh * mesh);
	void addNode(string name, vector<float> position, Mesh * mesh, Material * material);
	void drawNodes();
	void initUniforms();

	GLfloat modelmatrix[16]; /* Our model matrix  */

	/* An identity matrix we use to perform the equivalant of glLoadIdentity */

private:
    SceneGraph();

};
