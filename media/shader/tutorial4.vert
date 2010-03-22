#version 150

precision highp float;

in vec3 in_Position;
in vec3 in_Color;

out vec3 geom_Color;

void main(void) {
    gl_Position = vec4(in_Position, 1.0);
    geom_Color = in_Color;
}
