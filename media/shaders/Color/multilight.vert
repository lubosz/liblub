#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;

out vec4 positionView;
out vec3 normalView;
out vec3 normalWorld;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

void main() { 

    vec4 position = vec4(in_Vertex,1);

	normalView = normalize(NormalMatrix * in_Normal);
	normalWorld = in_Normal;
	positionView = MVMatrix * position;

	gl_Position = MVPMatrix * position;
}
