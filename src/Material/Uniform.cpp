/*
 * Copyright © 2010 Lubosz Sarnecki
 * Uniform.cpp
 *
 *  Created on: Dec 16, 2010
 */

#include "Uniform.h"

template <>
void Uniform<float>::init(GLuint program) {
	switch(values.size()) {
        case 1:
            glUniform1f(glGetUniformLocation(program, name.c_str()), values[0]);
            break;
        case 2:
            glUniform2f(glGetUniformLocation(
                    program, name.c_str()), values[0], values[1]);
            break;
        case 3:
            glUniform3f(glGetUniformLocation(
                    program, name.c_str()), values[0], values[1], values[2]);
            break;
        case 4:
            // TODO(bmonkey): vector methods
            // glUniform4fv(glGetUniformLocation(
            // program, name.c_str()), 4, values.data());
            glUniform4f(glGetUniformLocation(
                    program, name.c_str()),
                    values[0], values[1], values[2], values[3]);
		break;
	}
}

template <>
void Uniform<int>::init(GLuint program) {
	switch(values.size()) {
        case 1:
            glUniform1iv(glGetUniformLocation(
                    program, name.c_str()), 1, values.data());
            break;
        case 2:
            glUniform2iv(glGetUniformLocation(
                    program, name.c_str()), 2, values.data());
            break;
        case 3:
            glUniform3iv(glGetUniformLocation(
                    program, name.c_str()), 3, values.data());
            break;
        case 4:
            // glUniform4fv(glGetUniformLocation(
            // program, uniform.name.c_str()), 4, uniform.values.data());
            glUniform4i(glGetUniformLocation(
		        program, name.c_str()), values[0], values[1], values[2], values[3]);
		break;
	}
}
