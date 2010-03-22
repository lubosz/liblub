/*
 * RenderEngine.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <stdio.h>
#include <string.h>

//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>

//#include <string>

#include "RenderEngine.h"
#include "utils.h"

using namespace std;

RenderEngine::RenderEngine() {
	glError("RenderEngine",23);
	checkVersion();
	frameCount = 0;

	/* The four vericies of a tetrahedron */
	const GLfloat tetrahedron[4][3] = {
	{  1.0,  1.0,  1.0  },   /* index 0 */
	{ -1.0, -1.0,  1.0  },   /* index 1 */
	{ -1.0,  1.0, -1.0  },   /* index 2 */
	{  1.0, -1.0, -1.0  } }; /* index 3 */

	/* Color information for each vertex */
	const GLfloat colors[4][3] = {
	{  1.0,  0.0,  0.0  },   /* red */
	{  0.0,  1.0,  0.0  },   /* green */
	{  0.0,  0.0,  1.0  },   /* blue */
	{  1.0,  1.0,  1.0  } }; /* white */

	const GLubyte tetraindicies[6] = { 0, 1, 2, 3, 0, 1 };

    /* Call our function that performs opengl operations */

    /* Allocate and assign a Vertex Array Object to our handle */
    glGenVertexArrays(1, &vao);

    /* Bind our Vertex Array Object as the current used object */
    glBindVertexArray(vao);

    /* Allocate and assign three Vertex Buffer Objects to our handle */
    glGenBuffers(3, vbo);

    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    /* Copy the vertex data from tetrahedron to our buffer */
    /* 12 * sizeof(GLfloat) is the size of the tetrahedrom array, since it contains 12 GLfloat values */
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tetrahedron, GL_STATIC_DRAW);

    /* Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex */
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* Enable attribute index 0 as being used */
    glEnableVertexAttribArray(0);

    /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    /* Copy the color data from colors to our buffer */
    /* 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values */
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

    /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* Enable attribute index 1 as being used */
    glEnableVertexAttribArray(1);

    /* Bind our third VBO as being the active buffer and storing vertex array indicies */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);

    /* Copy the index data from tetraindicies to our buffer
     * 6 * sizeof(GLubyte) is the size of the index array, since it contains 6 GLbyte values */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLubyte), tetraindicies, GL_STATIC_DRAW);

    shaderProgram = new ShaderProgram();

    /* Create our projection matrix with a 45 degree field of view
     * a width to height ratio of 1 and view from .1 to 100 infront of us */
    perspective(projectionmatrix, 45.0, 1.0, 0.1, 100.0);
	glError("RenderEngine",92);
}

RenderEngine::~RenderEngine() {
	glError("RenderEngine",96);
    /* Cleanup all the things we bound and allocated */
	cout << "Shutting down Render Engine...";
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    delete shaderProgram;
    glDeleteBuffers(3, vbo);
    glDeleteVertexArrays(1, &vao);
	cout << "done.\n";
	glError("RenderEngine",106);
}

void RenderEngine::display(){
	//cout << frameCount << "\n";
	/* An identity matrix we use to perform the equivalant of glLoadIdentity */
	const GLfloat identitymatrix[16] = IDENTITY_MATRIX4;


    /* Load the identity matrix into modelmatrix. rotate the model, and move it back 5 */
      memcpy(modelmatrix, identitymatrix, sizeof(GLfloat) * 16);
      rotate(modelmatrix, (GLfloat)frameCount * -1.0, X_AXIS);
      rotate(modelmatrix, (GLfloat)frameCount * 1.0, Y_AXIS);
      rotate(modelmatrix, (GLfloat)frameCount * 0.5, Z_AXIS);
      translate(modelmatrix, 0, 0, -5.0);

      /* multiply our modelmatrix and our projectionmatrix. Results are stored in modelmatrix */
      multiply4x4(modelmatrix, projectionmatrix);

      /* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram->program, "mvpmatrix"), 1, GL_FALSE, modelmatrix);

      /* Make our background black */
      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /* Invoke glDrawElements telling it to draw a triangle strip using 6 indicies */
      glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, 0);

      frameCount++;
}

void RenderEngine::glError(string file, int line) {
        GLenum err (glGetError());

        while(err!=GL_NO_ERROR) {
                std::string error;

                switch(err) {
                        case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                        case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                        case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                        case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
                }

                std::cerr<<"GL_"<<error<<" - "<<file<<':'<<line<<std::endl;
                err=glGetError();
        }
}


void RenderEngine::checkVersion(){
	cout 	<< "OpenGL:\t" << glGetString(GL_VERSION) << "\n"
			<< "GLSL:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n"
			<< "Hardware:\t" << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER)
			//<< "GL_EXTENSIONS:\t" << glGetString(GL_EXTENSIONS) << "\n"
			<< "\n";
}

GLboolean RenderEngine::QueryExtension(char *extName)
{
  char *p = (char *) glGetString(GL_EXTENSIONS);
  char *end;
  if (p == NULL)
     return GL_FALSE;
  end = p + strlen(p);
  while (p < end) {
     int n = strcspn(p, " ");
     if ((strlen(extName)==n) && (strncmp(extName,p,n)==0)) {
       return GL_TRUE;
     }
     p += (n + 1);
  }
  return GL_FALSE;
}
