#version 330 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec4 positionView;
out vec3 normalView;
out vec2 uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

void main() { 
	uv = in_Uv;

    vec4 position = vec4(in_Vertex,1);
    
	normalView = normalize(NormalMatrix * in_Normal);
	positionView = MVMatrix * position;
	vec4 positionProjection = MVPMatrix * position;

	gl_Position = positionProjection;
}
