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

	cout << "Creating Shader " << fileName << "\n";
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

    //printShaderInfoLog(shader);
}

/* A simple function that will read a file into an allocated char pointer buffer */
char* Shader::readFile(string filePath)
{
    FILE *file;
    char *buffer;
    size_t result;
    long fileSize;

    file = fopen(filePath.c_str(), "r"); /* Open file for reading */
    if (!file){
    	cout << "File:" << filePath << "\n";
		fputs ("File error",stderr);
		exit (1);
    }

    // obtain file size:
    fseek (file , 0 , SEEK_END); /* Seek to the end of the file */
    fileSize = ftell (file); /* Find out how many bytes into the file we are */
    rewind (file); /* Go back to the beginning of the file */

    /* Allocate a buffer for the entire length of the file plus a null terminator */
    buffer = (char*) malloc (sizeof(char)*fileSize);
      if (!buffer) {fputs ("Memory error",stderr); exit (2);}

    result = fread(buffer, fileSize, 1, file); /* Read the contents of the file in to the buffer */

    /*
    //TODO: Occurs always
	if (result != fileSize){
    	cout << "File:" << filePath << "\n";
	    fputs ("Reading error",stderr); exit (3);
    }
    */

    fclose(file); /* Close the file */
    buffer[fileSize] = 0; /* Null terminator */

    return buffer; /* Return the buffer */
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

GLuint Shader::getReference() const{
	return shader;
}
