/*
 * Shader.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Shader.h"

Shader::Shader(string fileName, GLenum type) {

	this->fileName = fileName;
	this->type = type;

	loadAndCompile();

}

Shader::~Shader() {
	glDeleteShader(shader);
	free(source);
}


void Shader::loadAndCompile(){
	/* Read our shaders into the appropriate buffers */
	source = readFile(shaderDir + fileName);

    /* Assign our handles a "name" to new shader objects */
    shader = glCreateShader(type);

    /* Associate the source code buffers with each handle */
    glShaderSource(shader, 1, (const GLchar**)&source, 0);

    /* Compile our shader objects */
    glCompileShader(shader);

    printShaderInfoLog(shader);
}

/* A simple function that will read a file into an allocated char pointer buffer */
char* Shader::readFile(string file)
{
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file.c_str(), "r"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file plus a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

void Shader::printShaderInfoLog(GLuint shader) {
	int infologLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	//printOpenGLError();  // Check for OpenGL errors
	if (infologLen > 0) {
		infoLog = (GLchar*) malloc(infologLen);
		if (infoLog == NULL) {
			printf("ERROR: Could not allocate InfoLog buffer\n");
			exit(1);
		}
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		printf("Shader Log:\n%s\n\n", infoLog);
		free(infoLog);
	} else {
		printf("Shader Info Log: OK\n");
	}
	//printOpenGLError();  // Check for OpenGL errors
}

void Shader::reload(){
	glDeleteShader(shader);
	free(source);
	loadAndCompile();
}
