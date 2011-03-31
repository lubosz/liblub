#version 330 core

in vec3 in_Vertex;
out vec4 viewPosition;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;

void main(){	
	viewPosition = MVMatrix * vec4(in_Vertex,1);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
