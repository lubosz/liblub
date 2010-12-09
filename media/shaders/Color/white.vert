//#version 400 core
#version 330 core
//#version 130

//#define GL_ARB_gpu_shader_fp64    1
//#extension GL_NV_gpu_shader_fp64 : enable

//precision highp float;

in vec3 in_Vertex;

//uniform dmat4 MVPMatrix;
//uniform dmat4x4 MVPMatrix;
uniform mat4 MVPMatrix;

void main(void) {
	//gl_Position = MVPMatrix * f64vec4(in_Vertex,1);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
