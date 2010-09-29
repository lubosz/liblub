#version 330 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;

out vec4 positionCamView;
out vec3 normalCamView;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

void main() 
{ 

	positionCamView = MVMatrix * vec4(in_Vertex,1);
	normalCamView = NormalMatrix * in_Normal; 
	gl_Position = MVPMatrix * vec4(in_Vertex,1);

}
