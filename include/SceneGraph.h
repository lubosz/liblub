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
	vector<float> lightPosition;
	void updateLight();
	void drawNodes();

	void addNode(Node * node);
	void addNode(Mesh * mesh, vector<float> position, Material * material);
	void addNode(string file, vector<float> position, Material * material);

	void meshPlane(string file, float cubeSize, float step, vector<Material*> materials);

private:
	friend class Singleton<SceneGraph>;

	vector<Node> sceneNodes;
	vector<Material*> materials;

	GLfloat modelmatrix[16]; /* Our model matrix  */

	/* Multiply 4x4 matrix m1 by 4x4 matrix m2 and store the result in m1 */
	void multiply4x4(GLfloat *m1, GLfloat *m2);
	void transpose3x3(GLfloat *matrix);

	/* Perform translation operations on a matrix */
	void translate(GLfloat *matrix, GLfloat x, GLfloat y, GLfloat z);
	void translate(float x, float y, float z);
	void translate(vector<float> translation);

	/* Rotate a matrix by an angle on a X, Y, or Z axis specified by the AXIS enum*/
	void rotate(GLfloat *matrix, GLfloat angle, AXIS axis);
	void scale(GLfloat *matrix, GLfloat size);
	void scale(float size);

	void animate(float frameCount);
	void transform(float frameCount);
	void initNode();
	void cameraTransform();
	void bindShaders(ShaderProgram * shaderProgram);

	void initUniforms();
	void setPosition(string nodeName, vector<float> position);

	void setLightPosition(vector<float> lightPosition);

	/* An identity matrix we use to perform the equivalant of glLoadIdentity */

	void addNode(string name, string file, vector<float> position, Material * material);
	void addNode(string name, vector<float> position, Mesh * mesh);
	void addNode(string name, vector<float> position, Mesh * mesh, Material * material);

	void meshCube(string file, float cubeSize, float step, Material * material);
	void meshCube(string file, float cubeSize, float step, vector<Material*> materials);

    SceneGraph();

};
