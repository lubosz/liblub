#version 150 core

precision highp float;

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;

out vec3 geom_Color;

void main(void) {
    gl_Position = vec4(in_Position, 1.0);
    //geom_Color = in_Color;
    geom_Color = in_Normal;
}
