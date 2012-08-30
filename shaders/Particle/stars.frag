#version 330 core

precision highp float;

in  vec3 geom_Color;

out vec4 gl_FragColor;

void main(void) {
    gl_FragColor = vec4(geom_Color,1.0);
}
