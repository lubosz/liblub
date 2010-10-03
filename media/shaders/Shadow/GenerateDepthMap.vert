#version 330 core

in vec3 in_Vertex;

out vec4 position;

uniform mat4 MVPMatrix;

void main() 
{	
	position = MVPMatrix * vec4(in_Vertex,1);
	gl_Position = position;

}
