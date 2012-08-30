#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec4 positionCamView;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;

void main() 
{ 

	uv = in_Uv;
	positionCamView = MVMatrix * vec4(in_Vertex,1);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);

}
