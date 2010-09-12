#version 330 core

precision highp float;

in vec3 in_Vertex;

uniform mat4 MVPMatrix;

void main(void) {
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
