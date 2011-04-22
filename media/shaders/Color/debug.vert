#version 150 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec3 normal;
out vec2 uv;

void main()
{
	normal = in_Normal;
	uv = in_Uv;
    gl_Position     = MVPMatrix * vec4(in_Vertex,1);
}

