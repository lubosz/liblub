#version 330 core

//#define GL_ARB_gpu_shader_fp64    1

//precision highp float;

in vec3 in_Vertex;

//uniform dmat4 MVPMatrix;
uniform mat4 MVPMatrix;

void main(void) {
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
