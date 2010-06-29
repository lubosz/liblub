#version 150 core

precision highp float;

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_Normal;

out vec3 geom_Color;

uniform mat4 MVPMatrix;

void main(void) {
    geom_Color = in_Color;
    gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
