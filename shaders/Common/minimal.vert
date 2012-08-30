#version 330 core

in vec3 in_Vertex;

uniform mat4 MVPMatrix;

void main(){	
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
