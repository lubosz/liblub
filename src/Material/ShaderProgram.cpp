/*
 * ShaderSystem.cpp
 *
 *  Created on: Mar 22, 2010
 *      Author: bmonkey
 */

#include "ShaderProgram.h"


ShaderProgram::ShaderProgram() {
	attribCount = 0;
    /* Assign our program handle a "name" */
    program = glCreateProgram();
    cout << "Creating Program #" << program <<"\n";
}

ShaderProgram::~ShaderProgram() {
	cout << "Shutting down Shader System...";
    glUseProgram(0);

    while(shaders.size() > 0){
    	detachShader(shaders.back());
    }

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

void ShaderProgram::attachShader(string fileName, GLenum type){
    /* Attach our shaders to our program */
	Shader * shader = new Shader(fileName, type);
	shaders.push_back(shader);
	glAttachShader(program, shader->getReference());
}

void ShaderProgram::attachShader(string fileName, GLenum type, const vector<string> & defines){
    /* Attach our shaders to our program */
	Shader * shader = new Shader(fileName, type, defines);
	shaders.push_back(shader);
	glAttachShader(program, shader->getReference());
}

void ShaderProgram::detachShader(Shader *shader){
	glDetachShader(program, shader->getReference());
	shaders.remove(shader);
	delete shader;
}

void ShaderProgram::bindAttrib(unsigned position, string name){
	glBindAttribLocation(program, position, name.c_str());
}

void ShaderProgram::bindAttribIfUnbound(string name){
	//glBindAttribLocation(program, position, name.c_str());
	foreach( string attrib, boundAttribs )
	    {
	    	if (attrib == name) return;
	    }
	cout << "Binding " <<name <<"\n";
	boundAttribs.push_back(name);
	bindAttrib(name);
}



void ShaderProgram::bindAttrib(string name){
	bindAttrib(attribCount, name);
	attribCount++;
}

void ShaderProgram::linkAndUse(){
    /* Link our program, and set it as being actively used */
    glLinkProgram(program);
    printProgramInfoLog();
    use();
}

void ShaderProgram::use(){
	glError("ShaderProgram::use",101);
	glUseProgram(program);
	glError("ShaderProgram::use",103);
}

void ShaderProgram::reload(){
    glUseProgram(0);
    program = glCreateProgram();

    foreach( Shader* shader, shaders )
    {
    	/*

        delete shader;
        */
    	shader->reload();
    	//attachShader(shader->fileName,shader->type);
    }

    linkAndUse();
	//ShaderFactory::Instance().addUniforms();

}

void ShaderProgram::setUniform(const QVector4D& vector, string name){
	glUniform4f(glGetUniformLocation(program, name.c_str()), vector.x(), vector.y(),vector.z(),vector.w());
}

void ShaderProgram::setUniform(const QVector3D& vector, string name){
	glUniform3f(glGetUniformLocation(program, name.c_str()), vector.x(), vector.y(),vector.z());
}

void ShaderProgram::setUniform(const QVector2D& vector, string name){
	glUniform2f(glGetUniformLocation(program, name.c_str()), vector.x(), vector.y());
}

GLuint ShaderProgram::getReference() const{
	return program;
}

void ShaderProgram::setUniform(const QMatrix3x3 & matrix, string name){
    GLfloat mat[9];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 9; ++i)
        mat[i] = data[i];

	glUniformMatrix3fv(
			glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, mat
	);
	//glError("ShaderProgram::bindMatrix3x3",129);
}

void ShaderProgram::setUniform(const QMatrix4x4 & matrix, string name){

    GLfloat mat[16];
    const qreal *data = matrix.constData();
    for (int i = 0; i < 16; ++i)
        mat[i] = data[i];

	glUniformMatrix4fv(
			glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
			//reinterpret_cast<const GLfloat *>(matrix.constData())
			mat
	);
	/* DOUBLE PRECISION
	glUniformMatrix4dv(
			glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, matrix.data()
	);
	*/
	//glError("ShaderProgram::bindMatrix4x4",137);
}

void ShaderProgram::setUniform(float value, string name){
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}
