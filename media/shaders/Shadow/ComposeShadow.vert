#version 330 core

in vec3 in_Vertex;

out vec4 positionCamView;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;

void main() 
{ 

	positionCamView = MVMatrix * vec4(in_Vertex,1);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);

}
