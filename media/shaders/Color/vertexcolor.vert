#version 330 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_Normal;
in vec3 in_Uv;

out vec3 color;

uniform mat4 MVPMatrix;

void main(void) {
    color = in_Color;
    vec3 move = vec3(2*gl_InstanceID,0,0);
    gl_Position = MVPMatrix * vec4(in_Vertex+move,1);
}
