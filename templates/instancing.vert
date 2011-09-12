#version 330 core

//precision highp float;

in vec3 in_Vertex;
in vec3 in_Normal;
//in vec3 in_Tangent;
//in vec3 in_Bitangent;
in vec2 in_Uv;

out vec2 uv;
out vec3 viewNormal;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform positions {
	vec4 cubePosition[{{positionElements}}];
};

void main(void) {
    uv = in_Uv;
    viewNormal = NormalMatrix * in_Normal;
    vec4 move = vec4(cubePosition[gl_InstanceID]);
    gl_Position = MVPMatrix * (vec4(in_Vertex,1)+move);
}
