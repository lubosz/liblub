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
    Logger::Instance().message << "Creating Program #" << program;
    Logger::Instance().log("DEBUG","ShaderProgram");
}

ShaderProgram::~ShaderProgram() {
    Logger::Instance().log("MESSAGE","ShaderProgram","Shutting down Shader System...");
    glUseProgram(0);

    while(shaders.size() > 0){
    	detachShader(shaders.back());
    }
}

void ShaderProgram::printProgramInfoLog() {
	int infologLen = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 1) {
		GLchar * infoLog = (GLchar*) malloc(infologLen);
		if (infoLog == NULL) {
			Logger::Instance().log("ERROR","Program Log","Could not allocate InfoLog buffer");
		}
		int charsWritten = 0;
		glGetProgramInfoLog(program, infologLen, &charsWritten, infoLog);
		string shaderlog = infoLog;
		free(infoLog);
		Logger::Instance().log("ERROR","Program Log",shaderlog);
	} else {
		Logger::Instance().log("DEBUG","Program","Done");
	}
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

void ShaderProgram::attachVertFrag(string file) {
	attachShader(file + ".vert", GL_VERTEX_SHADER);
	attachShader(file + ".frag", GL_FRAGMENT_SHADER);
}
void ShaderProgram::attachVertFrag(string file, const vector<string> & defines){
	attachShader(file + ".vert", GL_VERTEX_SHADER, defines);
	attachShader(file + ".frag", GL_FRAGMENT_SHADER, defines);
}

void ShaderProgram::attachVertGeom(string file) {
	attachShader(file + ".vert", GL_VERTEX_SHADER);
	attachShader(file + ".geom", GL_GEOMETRY_SHADER);
}

void ShaderProgram::attachVertFragGeom(string file) {
	attachShader(file + ".vert", GL_VERTEX_SHADER);
	attachShader(file + ".geom", GL_GEOMETRY_SHADER);
	attachShader(file + ".frag", GL_FRAGMENT_SHADER);
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
	Logger::Instance().log("DEBUG","ShaderProgram", "Binding " + name);
	boundAttribs.push_back(name);
	bindAttrib(name);
}



void ShaderProgram::bindAttrib(string name){
	Logger::Instance().message << name << " #"<< attribCount;
	Logger::Instance().log("ShaderProgram", "Binding Shader Attribute");
	bindAttrib(attribCount, name);
	attribCount++;
}

void ShaderProgram::initUniforms(){
	foreach(Uniform uniform, uniforms){

		Logger::Instance().message << "Uniform: " + uniform.name + ": ";
		foreach(float value, uniform.values){
			Logger::Instance().message << value << ", ";
		}
		Logger::Instance().log("UNIFORMS", "ShaderProgram");

		//TODO: vector methods, uniform boilerplate
		switch(uniform.values.size()){
		case 1:
			glUniform1f(glGetUniformLocation(program, uniform.name.c_str()), uniform.values[0]);
			break;
		case 2:
			glUniform2f(glGetUniformLocation(program, uniform.name.c_str()), uniform.values[0],uniform.values[1]);
			break;
		case 3:
			glUniform3f(glGetUniformLocation(program, uniform.name.c_str()), uniform.values[0],uniform.values[1],uniform.values[2]);
			break;
		case 4:
//			glUniform4fv(glGetUniformLocation(program, uniform.name.c_str()), 4, uniform.values.data());
			glUniform4f(glGetUniformLocation(program, uniform.name.c_str()), uniform.values[0],uniform.values[1],uniform.values[2],uniform.values[3]);
			break;
		}

		Logger::Instance().message << uniform.values.size() << " bound.";

		Logger::Instance().log("UNIFORMS", "ShaderProgram");
		glError("ShaderProgram", 136);
	}
	foreach(Uniformi uniformi, uniformsi){

		Logger::Instance().message << "Uniform: " + uniformi.name + ": ";
		foreach(int value, uniformi.values){
			Logger::Instance().message << value << ", ";
		}
		Logger::Instance().log("UNIFORMSi", "ShaderProgram");

		switch(uniformi.values.size()){
		case 1:
			glUniform1iv(glGetUniformLocation(program, uniformi.name.c_str()), 1, uniformi.values.data());
			break;
		case 2:
			glUniform2iv(glGetUniformLocation(program, uniformi.name.c_str()), 2, uniformi.values.data());
			break;
		case 3:
			glUniform3iv(glGetUniformLocation(program, uniformi.name.c_str()), 3, uniformi.values.data());
			break;
		case 4:
//			glUniform4fv(glGetUniformLocation(program, uniform.name.c_str()), 4, uniform.values.data());
			glUniform4i(glGetUniformLocation(program, uniformi.name.c_str()), uniformi.values[0],uniformi.values[1],uniformi.values[2],uniformi.values[3]);
			break;
		}

		Logger::Instance().message << uniformi.values.size() << " bound.";

		Logger::Instance().log("UNIFORMSi", "ShaderProgram");
		glError("ShaderProgram", 165);
	}
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
