#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec4 positionCamView;
out vec3 normalCamView;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

void main() 
{ 

	positionCamView = MVMatrix * vec4(in_Vertex,1);
	normalCamView = NormalMatrix * in_Normal; 
	uv = in_Uv;
	gl_Position = MVPMatrix * vec4(in_Vertex,1);

}
