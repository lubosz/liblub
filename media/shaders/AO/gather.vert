#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
out vec4 viewPosition;
out vec3 viewNormal;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;

void main(){
	viewNormal = NormalMatrix * in_Normal;
	viewPosition = MVPMatrix * vec4(in_Vertex,1);
	gl_Position = viewPosition;
}
