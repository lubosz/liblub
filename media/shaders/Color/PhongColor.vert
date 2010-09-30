#version 330 core

//precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec4 positionView;
out vec3 normalView;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

void main() 
{ 

	positionView = MVMatrix * vec4(in_Vertex,1);
	normalView = NormalMatrix * in_Normal; 
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	uv = in_Uv;

}
