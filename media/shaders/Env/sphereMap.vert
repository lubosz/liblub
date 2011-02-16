#version 330 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

in vec3 in_Vertex;
in vec3 in_Normal;

out vec3 r;

void main() {
	vec3 u = vec3(MVMatrix * vec4(in_Vertex,1));
	vec3 n = normalize(NormalMatrix * in_Normal);
	r = reflect( u, n );

    gl_Position = MVPMatrix * vec4(in_Vertex,1);
}

