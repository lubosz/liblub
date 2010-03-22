/*
 * ShaderSystem.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {

	Shader * vertex = new Shader("tutorial4.vert", GL_VERTEX_SHADER);
	Shader * geomerty = new Shader("tutorial4.geom", GL_GEOMETRY_SHADER);
	Shader * fragment = new Shader("tutorial4.frag", GL_FRAGMENT_SHADER);
    /* Read our shaders into the appropriate buffers */

	/*
    vertexsource = filetobuf(shaderDir + "tutorial4.vert");
    geometrysource = filetobuf(shaderDir + "tutorial4.geom");
    fragmentsource = filetobuf(shaderDir + "tutorial4.frag");
	*/

    /* Assign our handles a "name" to new shader objects */
	/*
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    geometryshader = glCreateShader(GL_GEOMETRY_SHADER);
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    */

    /* Associate the source code buffers with each handle */
	/*
    glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
    glShaderSource(geometryshader, 1, (const GLchar**)&geometrysource, 0);
    glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
    */

    /* Compile our shader objects */

    /*
    glCompileShader(vertexshader);
    glCompileShader(geometryshader);
    glCompileShader(fragmentshader);

    printShaderInfoLog(vertexshader);
    printShaderInfoLog(geometryshader);
    printShaderInfoLog(fragmentshader);
*/

    /* Assign our program handle a "name" */
    program = glCreateProgram();

    attachShader(vertex);
    attachShader(geomerty);
    attachShader(fragment);

    /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");

    /* Link our program, and set it as being actively used */
    glLinkProgram(program);
    printProgramInfoLog();

    glUseProgram(program);

}

ShaderProgram::~ShaderProgram() {
	cout << "Shutting down Shader System...";
    glUseProgram(0);
    /*
    glDetachShader(shaderprogram, vertexshader);
    glDetachShader(shaderprogram, geometryshader);
    glDetachShader(shaderprogram, fragmentshader);
    glDeleteProgram(shaderprogram);
    glDeleteShader(vertexshader);
    glDeleteShader(geometryshader);
    glDeleteShader(fragmentshader);
    free(vertexsource);
    free(geometrysource);
    free(fragmentsource);
    */
	cout << "done.\n";
}

void ShaderProgram::printProgramInfoLog() {
	int infologLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLen);
	//printOpenGLError();  // Check for OpenGL errors
	if (infologLen > 0) {
		infoLog = (GLchar*) malloc(infologLen);
		if (infoLog == NULL) {
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}
		glGetProgramInfoLog(program, infologLen, &charsWritten, infoLog);
		printf("Program Log:\n%s\n\n", infoLog);
		free(infoLog);
	} else {
		printf("Program Info Log: OK\n");
	}
	//printOpenGLError();  // Check for OpenGL errors
}

void ShaderProgram::attachShader(Shader *shader){
    /* Attach our shaders to our program */
    //glAttachShader(shaderprogram, vertexshader);
    //glAttachShader(shaderprogram, geometryshader);
    //glAttachShader(shaderprogram, fragmentshader);
	glAttachShader(program, shader->shader);
}
