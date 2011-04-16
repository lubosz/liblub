#version 330 core

precision highp float;

in vec3 in_Vertex;
//in vec3 in_Normal;
//in vec3 in_Tangent;
//in vec3 in_Bitangent;
in vec2 in_Uv;

out vec2 uv;
uniform mat4 MVPMatrix;



uniform positions {
	vec4 cubePosition[4000];
};

void main(void) {
    uv = in_Uv;
    vec4 move = cubePosition[gl_InstanceID];
    gl_Position = MVPMatrix * (vec4(in_Vertex,1)+move);
}
