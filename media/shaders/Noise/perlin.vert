#version 410 core

in vec3 in_Vertex;
in vec2 in_Uv;

uniform mat4 MVPMatrix;

uniform float time;

out vec2 uv;

void main( void )
{
    uv = in_Uv;
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
